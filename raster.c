#include "raster.h"


/* 
Screen resolution 640 x 400 = 256,000 pixels
                  256,000/8 = 32,000 bytes
UINT32 = 4 bytes
UINT16 = 2 bytes
UINT8  = 1 byte

*/


void clear_screen(UINT32 *base){
    
    UINT32 i;
    
    // 32,000 bytes/4 bytes = 8000 32-bit words
    for (i = 0; i < 8000; i++){         
        base[i] = 0; 
    }

}

void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width){

    UINT16 r, c;
  
    // Prevent Out-of-Bounds
    if (row >= 400 || col >= 600){
        return;
    }
    if (row + width > 400){
        width = 400 - row;
    }
    if (col + length > 640){
        length = 640 - col;
    }

    for (r = 0; r < width; r++){
        
        for (c = 0; c < length; c++){

            clear_pixel(base, row + r, col + c);
        }

}

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col){

    UINT32 bytes_per_row = 640 / 8;
    // Prevent Out-of-Bounds
    if (row >= 400 || col >= 640){
        return;
    }


    UINT32 byte_index = row * bytes_per_row + (col / 8);
    UINT8 bit_mask = 1 << (7 - (col % 8));

    base[byte_index] |= bit_mask; 

}

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length){
  
    UINT8 *byte_base = (UINT8 *)base;
    UINT16 i;
    UINT32 bytes_per_row = 640/8;

    // Prevent Out-of-Bounds
    if (row >= 400 || col >= 640){
        return;
    }
    if (col + length > 640){
        length = 640 - col;
    }

    for (i = 0l i < length; i++){
        
        UINT32 byte_index = row * bytes_per_row + ((col + i) / 8);
        UINT8 bit_mask = 1 << (7 - ((col + i) % 8));

        byte_base[byte_index] |= bit_mask;
        
    }

}

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length){

    UINT8 *byte_base = (UINT8 *)base;
    UINT16 i;
    UINT32 bytes_per_row = 640/8;

    // Prevent Out-of-Bounds
    if (row >= 400 || col >= 640){
        return;
    }

    if (row + length > 400){
        length = 400 - row;
    }

    for (i = 0; i < length; i++){

        UINT32 byte_index = (row + i) * bytes_per_row + (col / 8);
        UINT8 bit_mask = 1 << (7 - (col % 8));

        byte_base[byte_index] |= bit_mask;

    }


}