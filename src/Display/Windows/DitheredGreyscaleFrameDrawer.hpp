#ifdef SYS_WINDOWS
#ifndef DITHEREDGREYSCALEFRAMEDRAWER_HPP
#define DITHEREDGREYSCALEFRAMEDRAWER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "WindowsTerminalManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace Display
{
    namespace Windows
    {
        static const uint32_t palette_dithered_greyscale[16] = {
            0x000000,
            0x111111,
            0x212121,
            0x333333,
            0x444444,
            0x565656,
            0x666666,
            0x777777,
            0x898989,
            0x999999,
            0xAAAAAA,
            0xBCBCBC,
            0xCCCCCC,
            0xDDDDDD,
            0xEFEFEF,
            0xFFFFFF,
        };

        const uint8_t palette_shades[16] = {0x00, 0x11, 0x21, 0x33, 0x44, 0x56, 0x66, 0x77, 0x89, 0x99, 0xAA, 0xBC, 0xCC, 0xDD, 0xEF, 0xFF};

        class DitheredGreyscaleFrameDrawer : public IFrameDrawer
        {
        private:
            std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
            WindowsTerminalManager terminal_manager;

            // Hashmap with all the CHAR_INFOS corresponding to each value of shade
            std::map<uint8_t, CHAR_INFO> shade_map;

            // Given an arbritary shade, gives the closest indexes in the palette to that shade, if there is an exact match both pair elements will be
            // the same
            std::pair<uint8_t, uint8_t> GetClosestIndexInPalette(uint8_t shade) const;
            void ComputeShadeMap();

        public:
            DitheredGreyscaleFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

            virtual void SetupFrameDrawer() override;

            virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

            virtual void DisplayFrame() override;

            virtual void ClearFrameBuffer() override;

            virtual void ReportInformation(const std::string& info) override;

            [[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
            [[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
        };
    }
}

#endif
#endif
