#pragma once

#include <vector>

namespace Math
{
    /* Types */
    template <typename T = float>
    using Array = std::vector<T>;
    template <typename T = float>
    using Matrix = std::vector<Array<T>>;
    using Image  = std::vector<std::vector<float>>;

    /* Constants */
    constexpr double Pi = 3.14159265359;
    constexpr glm::mat4 I4{1.f};

    template <typename T>
    double Gaussian(T x, T y, double sigma)
    {
        return exp(-(x * x + y * y) / (2.f * sigma * sigma)) / (2.f * Pi * sigma * sigma);
    }


    [[nodiscard]] inline Matrix<float> GaussianFilter(int height, int width, double sigma)
    {
        Matrix<float> kernel(height, Array<float>(width));
        float sum = 0.0;

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                kernel[i][j] = Gaussian(i, j, sigma);
                sum += kernel[i][j];
            }

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                kernel[i][j] /= sum;

        return kernel;
    }


    [[nodiscard]] inline Image GaussianSmoothing(Image &image)
    {
        auto filter{GaussianFilter(5, 5, 100.0)};

        int height = image.size();
        int width  = image[0].size();

        int filterHeight = filter.size();
        int filterWidth  = filter[0].size();

        int newImageHeight = height - filterHeight + 1;
        int newImageWidth  = width - filterWidth + 1;

        Image newImage(newImageHeight, Array<float>(newImageWidth, 0));

        for (int i = 0; i < newImageHeight; i++)
            for (int j = 0; j < newImageWidth; j++)
                for (int h = i; h < i + filterHeight; h++)
                    for (int w = j; w < j + filterWidth; w++)
                        newImage[i][j] += filter[h - i][w - j] * image[h][w];


        return newImage;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> ApplyFilter(Matrix<T> &src, Matrix<T> &filter)
    {
        if (filter.size() <= 0 || filter[0].size() <= 0)
            return {{}};

        int heightSrc    = src.size();
        int widthSrc     = src[0].size();
        int filterHeight = filter.size();
        int filterWidth  = filter[0].size();

        int newHeight = heightSrc - filterHeight + 1;
        int newWidth  = widthSrc - filterWidth + 1;

        auto out{Image(newHeight, Array<T>(newWidth, 0))};

        for (int i = 0; i < newHeight; i++)
            for (int j = 0; j < newWidth; j++)
                for (int h = i; h < i + filterHeight; h++)
                    for (int w = j; w < j + filterWidth; w++)
                        out[i][j] += filter[h - i][w - j] * src[h][w];
        return out;
    }


    template <typename T, int height, int width>
    struct GaussianKernel
    {
        std::vector<std::vector<T>> kernel;
        int h = height;
        int w = width;

        explicit GaussianKernel(T sigma) { kernel = std::move(GaussianFilter(height, width, sigma)); }

        [[nodiscard]] Image operator()(Image &image) { return ApplyFilter<float>(image, kernel); }
    };
} // namespace Math
