/*
 * Copyright 2021 University of Toronto
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors: Mario Badr, Sameh Attia and Tanner Young-Schultz
 */

#ifndef EZGL_COLOR_HPP
#define EZGL_COLOR_HPP

#include <cstdint>

namespace ezgl {

/**
 * Represents a color as a mixture or red, green, and blue as well as the transparency level.
 *
 * Each color channel and transparency level is an 8-bit value, ranging from 0-255.
 */
struct color {
  /**
   * Default constructor: Create a black color.
   */
  constexpr color() noexcept
      : red(0), green(0), blue(0), alpha(255)
  {
  }

  /**
   * Create a color.
   *
   * @param r The amount of red.
   * @param g The amount of green.
   * @param b The amount of blue.
   * @param a The level of transparency.
   */
  constexpr color(std::uint_fast8_t r,
      std::uint_fast8_t g,
      std::uint_fast8_t b,
      std::uint_fast8_t a = 255) noexcept
      : red(r), green(g), blue(b), alpha(a)
  {
  }

  /**
   * A red component of the color, between 0 and 255.
   */
  std::uint_fast8_t red;

  /**
   * The green component of the color, between 0 and 255.
   */
  std::uint_fast8_t green;

  /**
   * The blue component of the color, between 0 and 255.
   */
  std::uint_fast8_t blue;

  /**
   * The amount of transparency, between 0 and 255.
   */
  std::uint_fast8_t alpha;

  /**
   * Test for equality.
   */
  bool operator==(const color &rhs) const
  {
    return red == rhs.red && green == rhs.green && blue == rhs.blue && alpha == rhs.alpha;
  }

  /**
   * Test for inequality.
   */
  bool operator!=(const color &rhs) const
  {
    return !(rhs == *this);
  }
};
//building outline; AE AE AE
static constexpr color BUILD_STROKE(0xAE, 0xAE, 0xAE);
static constexpr color WHITE(0xFF, 0xFF, 0xFF);
static constexpr color BLACK(0x00, 0x00, 0x00);
static constexpr color GREY_55(0xED, 0xED, 0xED);
static constexpr color GREY_75(0xD2, 0xD2, 0xD2);
static constexpr color LIGHT_GREY(0xD2, 0xD2, 0xD2);
static constexpr color DARKER_GREY(0x2F, 0x5F, 0x5F);
static constexpr color RED(0xFF, 0x00, 0x00);
static constexpr color ORANGE(0xFF, 0xA5, 0x00);
static constexpr color PERI(0xA9, 0x6F, 0xB9);
static constexpr color YELLOW(0xFF, 0xFF, 0x00);
static constexpr color SAND(0xF2, 0xFA, 0xCF);
static constexpr color GREEN(0x00, 0xCC, 0x00);
static constexpr color CYAN(0x00, 0xFF, 0xFF);
static constexpr color BLUE(0x00, 0x00, 0xFF);
static constexpr color PURPLE(0xA0, 0x20, 0xF0);
static constexpr color PINK(0xFF, 0xC0, 0xCB);
static constexpr color LIGHT_PINK(0xFF, 0xB6, 0xC1);
static constexpr color NOTASDARK_GREEN(0x35, 0x9C, 0x35);
static constexpr color DARK_GREEN(0x00, 0x64, 0x00);
static constexpr color MAGENTA(0xFF, 0x00, 0xFF);
static constexpr color BISQUE(0xFF, 0xE4, 0xC4);
static constexpr color LIGHT_SKY_BLUE(0x33, 0x94, 0xDE);
static constexpr color LIGHT_BLUE(0xAD, 0xE1, 0xF7);
static constexpr color THISTLE(0xD8, 0xBF, 0xD8);
static constexpr color PLUM(0xDD, 0xA0, 0xDD);
static constexpr color KHAKI(0xF0, 0xE6, 0x8C);
static constexpr color CORAL(0xFF, 0x7F, 0x50);
static constexpr color TURQUOISE(0x40, 0xE0, 0xD0);
static constexpr color MEDIUM_PURPLE(0x93, 0x70, 0xDB);
static constexpr color DARK_SLATE_BLUE(0x48, 0x00, 0x8B);
static constexpr color DARK_KHAKI(0xBD, 0xB7, 0x6B);
static constexpr color TAN(0xAD, 0xA7, 0x3B);
static constexpr color LIGHT_MEDIUM_BLUE(0x44, 0x44, 0xFF);
static constexpr color SADDLE_BROWN(0x8B, 0x45, 0x13);
static constexpr color FIRE_BRICK(0xB2, 0x22, 0x22);
static constexpr color LIME_GREEN(0x32, 0xCD, 0x32);
static constexpr color HIGHWAY(0xFF, 0xA5, 0x00);
static constexpr color CUSTBLUE(0xFF, 0xFF, 0xFF);
//dark mode (dm) colours
static constexpr color dm_CANV(0x0C, 0x15, 0x24);
static constexpr color dm_POI(0x77, 0x72, 0xAB);
static constexpr color dm_LAKE(0x14, 0x27, 0x40);
static constexpr color dm_RIV(0x2F, 0x4E, 0x78);
static constexpr color dm_BUILD(0x2F, 0x4E, 0x78);
static constexpr color dm_HIGHWAY(0x00, 0x91, 0x8F);
static constexpr color dm_MAJOR(0x33, 0x6b, 0x6b);
static constexpr color dm_MINOR(0x48, 0x59, 0x59);
static constexpr color dm_INTER(0xD5, 0x02, 0xFA);
static constexpr color dm_findINTER(0xFF, 0x00, 0x73);
static constexpr color dm_STROKE(0x2c, 0x31, 0x38);





}

#endif //EZGL_COLOR_HPP
