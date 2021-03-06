

/*
 *
 * new Font
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : squarer.h
 * Date                : 09.01.2014
 * Font size in bytes  : 4623
 * Font width          : 10
 * Font height         : 11
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#ifndef SQUARER10X11_H
#define SQUARER10X11_H

//#define NEW_FONT_WIDTH 10
//#define NEW_FONT_HEIGHT 12



static const uint8_t squarer10x11[] PROGMEM = {
   0x20, 0xFA, // size
    0x0A, // width
    0x0C, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x00, 0x02, 0x05, 0x0A, 0x0A, 0x0A, 0x06, 0x02, 0x04, 0x04, 
    0x06, 0x06, 0x02, 0x08, 0x02, 0x03, 0x09, 0x05, 0x09, 0x0A, 
    0x09, 0x09, 0x09, 0x0A, 0x09, 0x09, 0x02, 0x02, 0x00, 0x08, 
    0x00, 0x09, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x09, 0x02, 0x05, 0x09, 0x0A, 0x0A, 0x09, 0x09, 0x09, 0x0A, 
    0x09, 0x09, 0x0A, 0x09, 0x09, 0x0A, 0x09, 0x09, 0x09, 0x04, 
    0x03, 0x04, 0x00, 0x08, 0x00, 0x09, 0x09, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x09, 0x02, 0x05, 0x09, 0x0A, 0x0A, 0x09, 0x09, 
    0x09, 0x0A, 0x09, 0x09, 0x0A, 0x09, 0x09, 0x0A, 0x09, 0x09, 
    0x09, 0x05, 0x02, 0x05, 0x00, 0x00, 
    
    // font data
    0x3F, 0x3F, 0x30, 0x30, // 33
    0x0F, 0x0F, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, // 34
    0xCC, 0xCC, 0xFF, 0xFF, 0xCC, 0xCC, 0xFF, 0xFF, 0xCC, 0xCC, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, // 35
    0x3F, 0x3F, 0x33, 0x33, 0xFF, 0xFF, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 36
    0x07, 0x07, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, // 37
    0xFE, 0xFE, 0xB7, 0xB7, 0x86, 0x86, 0x10, 0x10, 0x30, 0x30, 0x10, 0x10, // 38
    0x0F, 0x0F, 0x00, 0x00, // 39
    0xFF, 0xFF, 0x03, 0x03, 0x30, 0x30, 0x30, 0x30, // 40
    0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, // 41
    0x30, 0x78, 0xFC, 0xFC, 0x78, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 42
    0x30, 0x30, 0xFC, 0xFC, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 43
    0x00, 0x00, 0xF0, 0xF0, // 44
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 45
    0x00, 0x00, 0x30, 0x30, // 46
    0xE0, 0xFF, 0x1F, 0x30, 0x30, 0x00, // 47
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 48
    0x03, 0x03, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x30, 0x30, // 49
    0xF3, 0xF3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 50
    0x03, 0x03, 0x03, 0x33, 0x33, 0x33, 0x33, 0x33, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 51
    0x7F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 52
    0x3F, 0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 53
    0xFF, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0xF8, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 54
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 55
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 56
    0x7F, 0x7F, 0x63, 0x63, 0x63, 0x63, 0x63, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 57
    0x30, 0x30, 0x30, 0x30, // 58
    0x30, 0x30, 0xF0, 0xF0, // 59
    0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 61
    0x0F, 0x0F, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 63
    0xFF, 0xFF, 0x03, 0x7B, 0x7B, 0x63, 0x63, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 64
    0xFF, 0xFF, 0x63, 0x63, 0x63, 0x63, 0x63, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 65
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x3F, 0x3F, 0xF0, 0xF0, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 66
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 67
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, // 68
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0x03, 0x03, 0x03, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 69
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 70
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 71
    0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 72
    0xFF, 0xFF, 0x30, 0x30, // 73
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, // 74
    0xFF, 0xFF, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0x9F, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 75
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 76
    0xFF, 0xFF, 0x03, 0x03, 0x7F, 0x7F, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 77
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 78
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 79
    0xFF, 0xFF, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7F, 0x7F, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 80
    0xFF, 0xFF, 0x03, 0x03, 0xE3, 0xE3, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 81
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x3F, 0x3F, 0xF0, 0xF0, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 82
    0x3F, 0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 83
    0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, // 84
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 85
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, // 86
    0xFF, 0xFF, 0x00, 0x00, 0xF8, 0xF8, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 87
    0xCF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xCF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 88
    0x7F, 0x7F, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 89
    0xF3, 0xF3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 90
    0xFF, 0xFF, 0x01, 0x01, 0x30, 0x30, 0x20, 0x20, // 91
    0x1F, 0xFF, 0xE0, 0x00, 0x30, 0x30, // 92
    0x01, 0x01, 0xFF, 0xFF, 0x20, 0x20, 0x30, 0x30, // 93
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, // 95
    0xFF, 0xFF, 0x63, 0x63, 0x63, 0x63, 0x63, 0xFC, 0xFC, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 97
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x3F, 0x3F, 0xE0, 0xE0, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 98
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 99
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, // 100
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0x03, 0x03, 0x03, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 101
    0xFF, 0xFF, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 102
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 103
    0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 104
    0xFF, 0xFF, 0x30, 0x30, // 105
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, // 106
    0xFF, 0xFF, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0x9F, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 107
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 108
    0xFF, 0xFF, 0x03, 0x03, 0x7F, 0x7F, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 109
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 110
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 111
    0xFF, 0xFF, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7F, 0x7F, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 112
    0xFF, 0xFF, 0x03, 0x03, 0xE3, 0xE3, 0x03, 0x03, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 113
    0xFF, 0xFF, 0x63, 0x63, 0x63, 0x7F, 0x7F, 0xE0, 0xE0, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 114
    0x3F, 0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF3, 0xF3, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 115
    0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, // 116
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 117
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, // 118
    0xFF, 0xFF, 0x00, 0x00, 0xF0, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 119
    0x9F, 0xFF, 0x60, 0x60, 0x60, 0x60, 0x60, 0xFF, 0x9F, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 120
    0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, // 121
    0xFB, 0xFB, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1F, 0x1F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 122
    0x30, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x30, 0x30, 0x20, 0x20, // 123
    0xFF, 0xFF, 0x30, 0x30, // 124
    0x01, 0x01, 0xFF, 0xFF, 0x30, 0x20, 0x20, 0x30, 0x30, 0x00, // 125
};

#endif
