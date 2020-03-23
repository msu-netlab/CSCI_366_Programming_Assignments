//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.



#include <iostream>
#include <exception>

#ifndef BATTLESHIP_BITARRAY2D_HPP
#define BATTLESHIP_BITARRAY2D_HPP

using namespace std;

class BitArray2DException: public exception
{
private:
   char *cstr;

public:
   BitArray2DException(string message){
      cstr = new char[message.size() + 1];
      message.copy(cstr, message.size() + 1);
      cstr[message.size()] = '\0';
   }

   ~BitArray2DException(){
      delete cstr;
   }

   virtual const char* what() const throw(){
      return cstr;
   }
};


/**
 * Sets a bit in a two dimensional bit array
 * @param array - pointer to a bit array
 * @param row_width - the number of bits in a row of the two dimensional array
 * @param row - row index (0 indexed)
 * @param col - column index (0 indexed)
 */
extern "C" void set_bit_elem(char *array, unsigned int row_width, unsigned int row, unsigned int col);

/**
 * Gets a bit in a two dimensional bit array
 * @param array - pointer to a bit array
 * @param row_width - the number of bits in a row of the two dimensional array
 * @param row - row index (0 indexed)
 * @param col - column index (0 indexed)
 * @return a boolean containing the bit at [row][column]
 */
extern "C" bool get_bit_elem(char *array, unsigned int row_width, unsigned int row, unsigned int col);


class BitArray2D {
private:
   /**
    * The 'char' types does not mean that each bit is stored as a character - we just need the pointer
    * to have some type since there is no type for bit.
    */
   char* array = nullptr;

   unsigned int rows;
   unsigned int columns;

public:
   /**
    * Sets up the array to store rows * columns bits
    * @param rows - number of rows
    * @param columns - number of columns
    */
   BitArray2D(unsigned int rows, unsigned int columns);

   /**
    * Deallocate memory used for array
    */
   ~BitArray2D();

   /**
    * Get bit at row and column
    * @param row
    * @param column
    * @return bit at row and column as bool
    */
   bool get(unsigned int row, unsigned int column);

   /**
    * Set bit to true at row and column
    * @param row
    * @param column
    */
   void set(unsigned int row, unsigned int column);
};


#endif //BATTLESHIP_BIT_ARRAY_HPP
