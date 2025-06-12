#include <io/print.h>

#include <hal/put_char.h>

void printf(const char *format, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
                case 'c' :
                    {
                        char c = __builtin_va_arg(args, int);
                        os_rta_hal_putchar(c);
                        break;
                    }
                case 's' :
                    {
                        const char *str = __builtin_va_arg(args, const char *);
                        while (*str)
                        {
                            os_rta_hal_putchar(*str++);
                        }
                        break;
                    }

                case 'd' :
                    {
                        int num = __builtin_va_arg(args, int);
                        if (num < 0)
                        {
                            os_rta_hal_putchar('-');
                            num = -num;
                        }
                        char  buffer[12]; // Enough for 32-bit int
                        char *ptr = buffer + sizeof(buffer) - 1;
                        *ptr      = '\0';
                        do
                        {
                            *(--ptr)  = '0' + (num % 10);
                            num      /= 10;
                        } while (num);
                        while (*ptr)
                        {
                            os_rta_hal_putchar(*ptr++);
                        }
                        break;
                    }
                case 'x' :
                    {
                        unsigned int num = __builtin_va_arg(args, unsigned int);
                        char         buffer[20]; // Enough for 32-bit hex
                        char        *ptr = buffer + sizeof(buffer) - 1;
                        *ptr             = '\0';
                        do
                        {
                            unsigned int digit = num & 0xF;
                            *(--ptr)           = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
                            num >>= 4;
                        } while (num);
                        while (*ptr)
                        {
                            os_rta_hal_putchar(*ptr++);
                        }
                        break;
                    }
                case '%' :
                    {
                        os_rta_hal_putchar('%');
                        break;
                    }
                default :
                    {
                        // Handle unknown format specifier
                        os_rta_hal_putchar('%');
                        os_rta_hal_putchar(*format);
                        break;
                    }
            }
        }
        else
        {
            os_rta_hal_putchar(*format);
        }

        format++;
    }

    __builtin_va_end(args);
}
