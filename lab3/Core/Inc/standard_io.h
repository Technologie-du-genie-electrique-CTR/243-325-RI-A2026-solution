/*
 * standard_io.h
 *
 *  Created on: 3 sept. 2026
 *      Author: Utilisateur
 */

#ifndef INC_STANDARD_IO_H_
#define INC_STANDARD_IO_H_


/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#endif /* INC_STANDARD_IO_H_ */
