/* noisette
 * Copyright (C) 2019 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cmath>
#include <array>

namespace nst
{

constexpr auto Sqrt2 = 1.4142135623730950;
constexpr auto Pi = 3.1415926535897932;
constexpr auto MaxInt = static_cast<double>(std::numeric_limits<uint64_t>::max());
constexpr auto Gradients = std::array<std::pair<double, double>, 16>
{{
	{1.0, 0.0},
	{0.92387953251128674, 0.38268343236508978},
	{0.70710678118654757, 0.70710678118654746},
	{0.38268343236508984, 0.92387953251128674},
	{0.0, 1.0},
	{-0.38268343236508973, 0.92387953251128674},
	{-0.70710678118654746, 0.70710678118654757},
	{-0.92387953251128674, 0.38268343236508989},
	{-1.0, 0.0},
	{-0.92387953251128685, -0.38268343236508967},
	{-0.70710678118654768, -0.70710678118654746},
	{-0.38268343236509034, -0.92387953251128652},
	{0.0, -1.0},
	{0.38268343236509, -0.92387953251128663},
	{0.70710678118654735, -0.70710678118654768},
	{0.92387953251128652, -0.38268343236509039}
}};

constexpr uint64_t integer_noise(uint64_t i) noexcept
{
    i += 0x62a9d9ed799705f5;
    i ^= (i >> 33);
    i *= 0x4be98134a5976fd3;
    i ^= (i >> 33);
    i *= 0x3bc0993a5ad19a13;
    i ^= (i >> 33);
    return i;
}

constexpr uint64_t integer_noise_2d(uint64_t i, uint64_t j) noexcept
{
    return integer_noise(i + integer_noise(j));
}

constexpr uint64_t integer_noise_3d(uint64_t i, uint64_t j, uint64_t k) noexcept
{
    return integer_noise(i + integer_noise(j + integer_noise(k)));
}

constexpr double noise(uint64_t i) noexcept
{
    return static_cast<double>(integer_noise(i)) / MaxInt;
}

constexpr double noise_2d(uint64_t i, uint64_t j) noexcept
{
    return static_cast<double>(integer_noise_2d(i, j)) / MaxInt;
}

constexpr double noise_3d(uint64_t i, uint64_t j, uint64_t k) noexcept
{
    return static_cast<double>(integer_noise_3d(i, j, k)) / MaxInt;
}

constexpr double poly_interp(double t) noexcept
{
    return t * t * t * (10.0 + t * (-15.0 + t * 6.0));
}

constexpr double perlin_noise_2d(uint64_t seed, double x, double y) noexcept
{
    // WARNING: x and y must be positive
    auto i = static_cast<uint64_t>(x);
    auto j = static_cast<uint64_t>(y);
    auto u = x - static_cast<double>(i);
    auto v = y - static_cast<double>(j);
    // Gradients
    const auto& grad00 = Gradients[integer_noise_3d(seed, i    , j    ) % Gradients.size()];
    const auto& grad10 = Gradients[integer_noise_3d(seed, i + 1, j    ) % Gradients.size()];
    const auto& grad01 = Gradients[integer_noise_3d(seed, i    , j + 1) % Gradients.size()];
    const auto& grad11 = Gradients[integer_noise_3d(seed, i + 1, j + 1) % Gradients.size()];
    // Ramps
    auto n00 = grad00.first * u         + grad00.second * v;
    auto n10 = grad10.first * (u - 1.0) + grad10.second * v;
    auto n01 = grad01.first * u         + grad01.second * (v - 1.0);
    auto n11 = grad11.first * (u - 1.0) + grad11.second * (v - 1.0);
    // Interpolation
    auto t0 = poly_interp(u);
    auto n0 = (1.0 - t0) * n00 + t0 * n10;
    auto n1 = (1.0 - t0) * n01 + t0 * n11;
    auto t1 = poly_interp(v);
    return Sqrt2 * ((1.0 - t1) * n0 + t1 * n1);
}

constexpr double fractal_noise_2d(uint64_t seed, double x, double y, unsigned int octaves, double lacunarity, double persistence) noexcept
{
    auto value = 0.0;
    auto frequency = 1.0;
    auto amplitude = 1.0;
    for (auto i = 0u; i < octaves; ++i)
    {
        value += amplitude * perlin_noise_2d(seed, frequency * x, frequency * y);
        frequency *= lacunarity;
        amplitude *= persistence;
    }
    return value;
}

}