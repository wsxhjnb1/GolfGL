#version 330 core
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D velocityTexture;
uniform vec2 frameBufSize;
in vec2 TexCoords;

uniform bool AA;

uniform float maxBlurAmount = 1.0;

float FxaaLuma(vec3 rgb) 
{
    return rgb.g * 1.9632107f + rgb.r;
}

vec4 FxaaTexOff(vec2 offset) 
{
    return texture2D(screenTexture, TexCoords + offset / frameBufSize);
}

vec3 FXAA()
{      
    const float FXAA_EDGE_THRESHOLD      = 1.f/8.f;   // Minimum local contrast required to apply algorithm
    const float FXAA_EDGE_THRESHOLD_MIN  = 1.f/24.f;  // Trims the algorithm from processing darks
    const float FXAA_SEARCH_STEPS        = 32.f;      // Maximum number of search steps
    const float FXAA_SEARCH_THRESHOLD    = 0.25f;     // Controls search stopping
    const float FXAA_SUBPIX_TRIM         = 0.25f;     // Controls removal of sub-pixel aliasing
    const float FXAA_SUBPIX_CAP          = 0.75f;     // Insures fine detail is not completely removed (partly overrides FXAA_SUBPIX_TRIM)

    const float FXAA_SUBPIX_TRIM_SCALE   = 1.f / (1.f - FXAA_SUBPIX_TRIM);

    // CONTRAST TEST
    vec3 rgbN = FxaaTexOff(vec2( 0.f,-1.f)).xyz;
    vec3 rgbW = FxaaTexOff(vec2(-1.f, 0.f)).xyz;
    vec3 rgbE = FxaaTexOff(vec2( 1.f, 0.f)).xyz;
    vec3 rgbS = FxaaTexOff(vec2( 0.f, 1.f)).xyz;
    vec3 rgbM = texture2D(screenTexture, TexCoords).xyz;
    
    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    
    float range = rangeMax - rangeMin;
    if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))            
        return rgbM;            
    
    // SUB-PIXEL ALIASING TEST
    vec3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
    
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25f;
    float rangeL = abs(lumaL - lumaM);
    float blendL = max(0.0, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
    blendL = min(FXAA_SUBPIX_CAP, blendL);
    
    vec3 rgbNW = FxaaTexOff(vec2(-1.f, -1.f)).xyz;
    vec3 rgbNE = FxaaTexOff(vec2( 1.f, -1.f)).xyz;
    vec3 rgbSW = FxaaTexOff(vec2(-1.f,  1.f)).xyz;
    vec3 rgbSE = FxaaTexOff(vec2( 1.f,  1.f)).xyz;
    rgbL += rgbNW + rgbNE + rgbSW + rgbSE;
    rgbL *= (1.f/9.f);
    
    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    
    // EDGE CLASSIFICATION
    float edgeVert = abs(0.25f * lumaNW + -0.5f * lumaN + 0.25f * lumaNE)
                   + abs(0.5f  * lumaW  + -1.f  * lumaM + 0.5f  * lumaE)
                   + abs(0.25f * lumaSW + -0.5f * lumaS + 0.25f * lumaSE);
    float edgeHorz = 
        abs(0.25f * lumaNW + -0.5f * lumaW + 0.25f * lumaSW)
      + abs(0.5f  * lumaN  + -1.f  * lumaM + 0.5f  * lumaS )
      + abs(0.25f * lumaNE + -0.5f * lumaE + 0.25f * lumaSE);
        
    bool horzSpan = edgeHorz >= edgeVert;
    vec2 rFrameBufSize = 1.f / frameBufSize;
    float lengthSign = horzSpan ? -rFrameBufSize.y : -rFrameBufSize.x;
    
    if(!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }
    
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5f;
    lumaS = (lumaS + lumaM) * 0.5f;
    
    if (gradientN < gradientS)
    {
        lumaN = lumaS;
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.f;
    }
    
    vec2 posN;
    posN.x = TexCoords.x + (horzSpan ? 0.f : lengthSign * 0.5f);
    posN.y = TexCoords.y + (horzSpan ? lengthSign * 0.5f : 0.f);
    
    gradientN *= FXAA_SEARCH_THRESHOLD;
    
    vec2 posP = posN;     
    vec2 offNP = horzSpan ? vec2(rFrameBufSize.x, 0.f) : vec2(0.f, rFrameBufSize.y);
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    posN += offNP * vec2(-1.f, -1.f);
    posP += offNP * vec2( 1.f,  1.f);
    
    // END-OF-EDGE SEARCH
    for(int i = 0; i < FXAA_SEARCH_STEPS; i++) 
    {
        if(!doneN)        
            lumaEndN = FxaaLuma(texture2D(screenTexture, posN.xy).xyz);
        
        if(!doneP)        
            lumaEndP = FxaaLuma(texture2D(screenTexture, posP.xy).xyz);
        
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        
        if(doneN && doneP)        
            break;        
        if(!doneN)        
            posN -= offNP;        
        if(!doneP)        
            posP += offNP;        
    }
    
    float dstN = horzSpan ? TexCoords.x - posN.x : TexCoords.y - posN.y;
    float dstP = horzSpan ? posP.x - TexCoords.x : posP.y - TexCoords.y;
    bool directionN = dstN < dstP;
    lumaEndN = directionN ? lumaEndN : lumaEndP;
    
    if((lumaM - lumaN) < 0.f == (lumaEndN - lumaN) < 0.f)    
        lengthSign = 0.f; 

    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5f + (dstN * (-1.f/spanLength))) * lengthSign;
    vec3 rgbF = texture2D(screenTexture, vec2(
        TexCoords.x + (horzSpan ? 0.f : subPixelOffset),
        TexCoords.y + (horzSpan ? subPixelOffset : 0.f))).xyz;
    
    return mix(rgbF, rgbL, blendL);    
}  

vec4 applyMotionBlur(vec4 color)
{
    vec3 velocity = texture2D(velocityTexture, TexCoords).rgb;
    float speed = length(velocity);
    float blurAmount = clamp(speed * 5.0, 0.0, maxBlurAmount); 

    vec4 blurredColor = vec4(0.0);
    int samples = int(blurAmount);
    vec2 texOffset = velocity.xy / textureSize(screenTexture, 0);

    for (int i = -samples; i <= samples; ++i)
    {
        vec2 offset = TexCoords + texOffset * float(i) / maxBlurAmount;
        blurredColor += texture2D(screenTexture, offset);
    }

    blurredColor /= (samples * 2 + 1);

    return mix(color, blurredColor, clamp(speed, 0.0, 1.0));
}

void main()
{
    vec4 color;
    if (AA)
    {
        color = vec4(FXAA(), 1.0);
    }
    else
    {
        color = texture2D(screenTexture, TexCoords);
    }

    FragColor = applyMotionBlur(color);
}