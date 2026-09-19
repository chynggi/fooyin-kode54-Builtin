/*
 * MIDI Plugin
 * Copyright © 2025, Christopher Snowhill <kode54@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

namespace Fooyin::VGMStreamInput {
/*! Mirrors the loop radio buttons of the foobar2000 component. */
enum class LoopMode
{
    Normal = 0, //!< Play loop_count loops, then fade.
    Forever,    //!< Keep looping; only meaningful for files with loop points.
    Ignore,     //!< Ignore loop points and play the stream once.
};

constexpr auto DefaultLoopMode        = static_cast<int>(LoopMode::Normal);
constexpr auto LoopModeKey            = "VGMStreamInput/LoopMode";
constexpr auto DefaultLoopCount       = 2.0;
constexpr auto LoopCount              = "VGMStreamInput/LoopCount";
constexpr auto DefaultFadeLength      = 4000;
constexpr auto FadeLength             = "VGMStreamInput/FadeLength";
constexpr auto DefaultFadeDelay       = 0;
constexpr auto FadeDelay              = "VGMStreamInput/FadeDelay";
constexpr auto DefaultDownmixChannels = 0;
constexpr auto DownmixChannels        = "VGMStreamInput/DownmixChannels";
constexpr auto DefaultDisableSubsongs = false;
constexpr auto DisableSubsongs        = "VGMStreamInput/DisableSubsongs";
constexpr auto DefaultDisableTagfile  = false;
constexpr auto DisableTagfile         = "VGMStreamInput/DisableTagfile";
constexpr auto DefaultCommonExts      = false;
constexpr auto CommonExts             = "VGMStreamInput/CommonExtensions";

} // namespace Fooyin::VGMStreamInput
