
 /* ******************************************************************************
  * BSD 2-Clause License
  *
  * Copyright (c) 2017, Brig Young, All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *
  * Redistributions of source code must retain the above copyright notice, this
  * list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  *
  * THIS SOFTWARE IS PROVIDED BY BRIG YOUNG AND CONTRIBUTORS "AS IS" AND ANY
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL BRIG YOUNG OR CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ******************************************************************************
  */

#include "megafastports.h"     // Prototypes for our API functions

// TODO:
/* ******************************************************************************
 * BASIC API FUNCTIONS IMPLEMENTATIONS
 * ******************************************************************************
 */

/*unsigned char mfp_Mode(unsigned char pin_number,
                         unsigned char mask_mode)
{
   return MASK_FAILURE;
}

unsigned char mfp_Read(unsigned char pin_number)
{
   return MASK_FAILURE;
}

unsigned char mfp_Write(unsigned char pin_number,
                           unsigned char mask_pin)
{
   return MASK_FAILURE;
}
*/
// TODO:
/* ******************************************************************************
 * ADVANCED API FUNCTION IMPLEMENTATIONS
 * ******************************************************************************
 */

// FUNCTION: readMegaMasked() Reads data_read from register_name.
// Returns: data_read after applying mask_value according to mask_mode 
unsigned char mfp_ReadMasked(unsigned char register_name, unsigned char mask_mode, unsigned char mask_value) {
   unsigned char data_read = 0;
   unsigned char data_masked = 0;

   switch (register_name) { 
      case REGISTER_X: {
         DDR_X &= ~(MASK_REG_X);    // Set AVRMega pins to receive input (1 clock)
         data_read = (PIN_X);       // Capture data bits                 (1 clock)
         data_read &= (MASK_REG_X); // Only save OUR bits                (1 clock)
         break;                     //                                   (1 clock)
      }
      case REGISTER_Y: {
         DDR_Y &= ~(MASK_REG_Y);
         data_read = (PIN_Y);
         data_read &= (MASK_REG_Y);
         break;
      }
      case REGISTER_Z: {
         DDR_Z &= ~(MASK_REG_Z);
         data_read = (PIN_Z);
         data_read &= (MASK_REG_Z);
         break;
      }
      case REGISTER_CTRL: {            // ONLY USE THE LOWER 4 BITS OF REGISTER_CTRL
         DDR_CTRL &= ~(MASK_REG_CTRL); // CRITICAL: Set ONLY our AVRMega Pins to receive input
         data_read = (PIN_CTRL);       // Capture data bits from entire register
         data_read &= (MASK_REG_CTRL); // Clear data bits in the high nibble.
         break;
      }
      case REGISTER_FLAG: {            // ONLY USE THE LOWER 4 BITS OF REGISTER_FLAG
         DDR_FLAG &= ~(MASK_REG_FLAG); // CRITICAL: Set ONLY our AVRMega Pins to receive input
         data_read = (PIN_FLAG);
         data_read &= (MASK_REG_FLAG);
         break;
      }
      default : {
         Serial.println("ERROR: BAD reg_name VALUE TO __FUNC__, __LINE__");
         Serial.print("register_name value: "); Serial.println(register_name);
         debugRegisters();
         return MASK_FAILURE;
      }
   } // End switch (reg_name)
   switch(mask_mode) {
      case MASK_MODE_AND: {
         data_masked = (data_value & mask_value);
         return data_masked;
         break;
      }
      case MASK_MODE_OR: {
         data_masked = (data_value | mask_value);
         return data_masked;
         break;
      }
      case MASK_MODE_XOR: {
         data_masked = (data_value ^ mask_value);
         return data_masked;
         break;
      }
      case MASK_MODE_NOT: {
         data_masked = ((~data_value) & MASK_FULL);
         return data_masked;
         break;
      }
      case MASK_MODE_NAND: {
         data_masked = ( ~((data_value) & (mask_value)) );
         return data_masked;
         break;
      }
      case MASK_MODE_NOR: {
         data_masked = ( ~((data_value) | (mask_value)) );
         return data_masked;
         break;
      }
      default : {
         return MASK_FAILURE;
      }
   } // End switch (mask_mode)
}

// FUNCTION: writeMegaMasked()
unsigned char mfp_WriteMasked(unsigned char register_name,
                             unsigned char mask_mode,
                             unsigned char mask_value,
                             unsigned char data_value) {
   unsigned char data_write = 0;
   unsigned char data_masked = 0;

   switch(mask_mode) {
      case MASK_MODE_AND: {
         data_masked = (data_value & mask_value);
         break;
      }
      case MASK_MODE_OR: {
         data_masked = (data_value | mask_value);
         break;
      }
      case MASK_MODE_XOR: {
         data_masked = (data_value ^ mask_value);
         break;
      }
      case MASK_MODE_NOT: {
         data_masked = ((~data_value) & MASK_FULL);
         break;
      }
      case MASK_MODE_NAND: {
         data_masked = ( ~((data_value) & (mask_value)) );
         break;
      }
      case MASK_MODE_NOR: {
         data_masked = ( ~((data_value) | (mask_value)) );
         break;
      }
      default : {
         return MASK_FAILURE;
      }
   } // End switch (mask_mode)

   switch (register_name) {
      case REGISTER_X: {
         data_write = ( (data_masked) & (MASK_REG_X) ); // Trim data with register's mask
         DDR_X |= (MASK_REG_X);                       // Set our data register to source output
         REG_X ^= REG_X;                              // Clear register
         REG_X |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      case REGISTER_Y: {
         data_write = ( (data_masked) & (MASK_REG_Y) );
         DDR_Y |= (MASK_REG_Y);
         REG_Y ^= REG_Y;
         REG_Y |= data_write;
         return MASK_SUCCESS;
      }
      case REGISTER_Z: {
         data_write = ( (data_masked) & (MASK_REG_Z) );
         DDR_Z |= (MASK_REG_Z);
         REG_Z ^= REG_Z;
         REG_Z |= data_write;
         return MASK_SUCCESS;
      }
      case REGISTER_CTRL: {
         data_write = ( (data_masked) & (MASK_REG_CTRL) ); // Trim data with register's mask
         DDR_CTRL |= (MASK_REG_CTRL);                    // Set our data register to source output
         REG_CTRL &= ~(MASK_REG_CTRL);                   // Clear ONLY our bits, not entire register ;-)
         REG_CTRL |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      case REGISTER_FLAG: {
         data_write = ( (data_masked) & (MASK_REG_FLAG) );
         DDR_FLAG |= (MASK_REG_FLAG);
         REG_FLAG &= ~(MASK_REG_FLAG);                   // Clear ONLY our bits, not entire register
         REG_FLAG |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      default : {
         Serial.println("ERROR: BAD reg_name VALUE TO __FUNC__, __LINE__");
         Serial.print("register_name value: "); Serial.println(register_name);
         debugRegisters();
         return MASK_FAILURE;
      }
   } // End switch (reg_name)

}


// FINISHED:
/* ******************************************************************************
 * BASIC API FUNCTION IMPLEMENTATIONS
 * ******************************************************************************
 */

// FUNCTION: readMega() Sets reg_name to input, sets data_read to port value.
// Returns: data_read.
unsigned char mfp_Read(unsigned char reg_name) {
   unsigned char data_read = 0;

   switch (reg_name) { 
      case REGISTER_X: {
         DDR_X &= ~(MASK_REG_X);    // Set AVRMega pins to receive input (1 clock)
         data_read = (PIN_X);       // Capture data bits                 (1 clock)
         data_read &= (MASK_REG_X); // Only save OUR bits                (1 clock)
         break;                     //                                   (1 clock)
      }
      case REGISTER_Y: {
         DDR_Y &= ~(MASK_REG_Y);
         data_read = (PIN_Y);
         data_read &= (MASK_REG_Y);
         break;
      }
      case REGISTER_Z: {
         DDR_Z &= ~(MASK_REG_Z);
         data_read = (PIN_Z);
         data_read &= (MASK_REG_Z);
         break;
      }
      case REGISTER_CTRL: {            // ONLY USE THE LOWER 4 BITS OF REGISTER_CTRL
         DDR_CTRL &= ~(MASK_REG_CTRL); // CRITICAL: Set ONLY our AVRMega Pins to receive input
         data_read = (PIN_CTRL);       // Capture data bits from entire register
         data_read &= (MASK_REG_CTRL); // Clear data bits in the high nibble.
         break;
      }
      case REGISTER_FLAG: {            // ONLY USE THE LOWER 4 BITS OF REGISTER_FLAG
         DDR_FLAG &= ~(MASK_REG_FLAG); // CRITICAL: Set ONLY our AVRMega Pins to receive input
         data_read = (PIN_FLAG);
         data_read &= (MASK_REG_FLAG);
         break;
      }
      default : {
         Serial.println("ERROR: BAD reg_name VALUE TO __FUNC__, __LINE__");
         Serial.print("reg_name value: "); Serial.println(reg_name);
         debugRegisters();
         return MASK_FAILURE;
      }
   } // End switch (reg_name)
   return data_read;
} // End readMega()


// FUNCTION: writeMega() Writes the given data_byte to the given reg_name.
// Returns: MASK_SUCCESS on SUCCESS; MASK_FAILURE on FAILURE
unsigned char mfp_Write(unsigned char reg_name, unsigned char data_byte) {
   unsigned char data_write = 0;

   switch (reg_name) {
      case REGISTER_X: {
         data_write = ( (data_byte) & (MASK_REG_X) ); // Trim data with register's mask
         DDR_X |= (MASK_REG_X);                       // Set our data register to source output
         REG_X ^= REG_X;                              // Clear register
         REG_X |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      case REGISTER_Y: {
         data_write = ( (data_byte) & (MASK_REG_Y) );
         DDR_Y |= (MASK_REG_Y);
         REG_Y ^= REG_Y;
         REG_Y |= data_write;
         return MASK_SUCCESS;
      }
      case REGISTER_Z: {
         data_write = ( (data_byte) & (MASK_REG_Z) );
         DDR_Z |= (MASK_REG_Z);
         REG_Z ^= REG_Z;
         REG_Z |= data_write;
         return MASK_SUCCESS;
      }
      case REGISTER_CTRL: {
         data_write = ( (data_byte) & (MASK_REG_CTRL) ); // Trim data with register's mask
         DDR_CTRL |= (MASK_REG_CTRL);                    // Set our data register to source output
         REG_CTRL &= ~(MASK_REG_CTRL);                   // Clear ONLY our bits, not entire register ;-)
         REG_CTRL |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      case REGISTER_FLAG: {
         data_write = ( (data_byte) & (MASK_REG_FLAG) );
         DDR_FLAG |= (MASK_REG_FLAG);
         REG_FLAG &= ~(MASK_REG_FLAG);                   // Clear ONLY our bits, not entire register
         REG_FLAG |= data_write;                         // write ONLY our bits to register
         return MASK_SUCCESS;
      }
      default : {
         Serial.println("ERROR: BAD reg_name VALUE TO __FUNC__, __LINE__");
         debugRegisters();
         return MASK_FAILURE;
      }
   } // End switch (reg_name)
} // End writeMega()


// FUNCTION: debugRegisters() Prints the values of Registers _X, _Y, _Z, _CTRL and _FLAG on Serial0
// Returns: void
// TODO: Re-Write to return a formatted string.
void mfp_debugRegisters() {
Serial.print("\n==============================================\n");
Serial.println("Begin register diagnostics");
Serial.println("Output Register, Direction Register, Input Register");
Serial.println("Register X");
Serial.println(REG_X, BIN);
Serial.println(DDR_X, BIN);
Serial.println(PIN_X, BIN);
Serial.println("Register Y");
Serial.println(REG_Y, BIN);
Serial.println(DDR_Y, BIN);
Serial.println(PIN_Y, BIN);
Serial.println("Register Z");
Serial.println(REG_Z, BIN);
Serial.println(DDR_Z, BIN);
Serial.println(PIN_Z, BIN);
Serial.println("Flags Register");
Serial.println(REG_FLAG & MASK_REG_FLAG, BIN);
Serial.println(DDR_FLAG & MASK_REG_FLAG, BIN);
Serial.println(PIN_FLAG & MASK_REG_FLAG,  BIN);
Serial.println("Control Register");
Serial.println(REG_CTRL & MASK_REG_CTRL, BIN);
Serial.println(DDR_CTRL & MASK_REG_CTRL, BIN);
Serial.println(PIN_CTRL & MASK_REG_CTRL, BIN);
Serial.println("End register diagnostics");
Serial.print(  "==============================================\n\n");
} // End debugRegisters()


// FUNCTION: debugRegister() Prints the value of register reg_name on Serial0
// Returns: void
// TODO: Re-Write to return a formatted string.
void mfp_debugRegister(unsigned char reg_name) {
   Serial.print("\n==============================================\n");
   Serial.println("Begin register diagnostics");
   Serial.println("Output Register, Direction Register, Input Register");
   switch(reg_name) {
   case REGISTER_X:
      Serial.println("Register X");
      Serial.println(REG_X, BIN);
      Serial.println(DDR_X, BIN);
      Serial.println(PIN_X, BIN);
      break;
   case REGISTER_Y:
      Serial.println("Register Y");
      Serial.println(REG_Y, BIN);
      Serial.println(DDR_Y, BIN);
      Serial.println(PIN_Y, BIN);
      break;
   case REGISTER_Z:
      Serial.println("Register Z");
      Serial.println(REG_Z, BIN);
      Serial.println(DDR_Z, BIN);
      Serial.println(PIN_Z, BIN);
      break;
   case REGISTER_CTRL:
      Serial.println("Control Register");
      Serial.println(REG_CTRL & MASK_REG_CTRL, BIN);
      Serial.println(DDR_CTRL & MASK_REG_CTRL, BIN);
      Serial.println(PIN_CTRL & MASK_REG_CTRL, BIN);
      break;
   case REGISTER_FLAG:
      Serial.println("Flags Register");
      Serial.println(REG_FLAG & MASK_REG_FLAG, BIN);
      Serial.println(DDR_FLAG & MASK_REG_FLAG, BIN);
      Serial.println(PIN_FLAG & MASK_REG_FLAG,  BIN);
      break;
   default:
      debugRegisters();
      break;
   }
   Serial.println("End register diagnostics");
   Serial.print(  "==============================================\n\n");
} // End debugRegister()



