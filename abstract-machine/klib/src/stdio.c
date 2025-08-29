#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static int vsnprintf_internal(char *out, size_t size, const char *fmt, va_list args) {
	char *p = out;

	while (*fmt) {
		if (*fmt != '%') {
		*p++ = *fmt++;
		continue;
		}

		fmt++; // Skip '%'
		switch (*fmt++) {
		case 'd': {
			int val = va_arg(args, int);
			char buf[20];
			char *b = buf + sizeof(buf) - 1;
			*b = '\0';
			int neg = val < 0;
			unsigned u = neg ? -val : val;
			do {
				*--b = '0' + (u % 10);
				u /= 10;
			} while (u);
			if (neg) *--b = '-';
			while (*b) *p++ = *b++;
			break;
		}
		case 's': {
			const char *str = va_arg(args, const char *);
			while (*str) *p++ = *str++;
			break;
		}
		case 'c': {
			*p++ = (char)va_arg(args, int);
			break;
		}
		case '%': {
			*p++ = '%';
			break;
		}
		default:
			// Unsupported format specifier
			*p++ = '?';
			break;
		}
	}

	*p = '\0';
	return p - out;  // Return number of characters written
}

int printf(const char *fmt, ...) {
    char buf[1024]; // temporary buffer
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf_internal(buf, sizeof(buf), fmt, args);
    va_end(args);
    putstr(buf);
    return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    return vsnprintf_internal(out, (size_t)-1, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vsnprintf_internal(out, (size_t)-1, fmt, args);
    va_end(args);
    return ret;
}

/*
int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *p = out;

  while (*fmt) {
    if (*fmt != '%') {
      *p++ = *fmt++;
      continue;
    }

    fmt++; // Skip '%'
    switch (*fmt++) {
      case 'd': {
        int val = va_arg(args, int);
        char buf[20];
        char *b = buf + sizeof(buf) - 1;
        *b = '\0';
        int neg = val < 0;
        unsigned u = neg ? -val : val;
        do {
            *--b = '0' + (u % 10);
            u /= 10;
        } while (u);
        if (neg) *--b = '-';
        while (*b) *p++ = *b++;
        break;
      }
      case 's': {
        const char *str = va_arg(args, const char *);
        while (*str) *p++ = *str++;
        break;
      }
      case 'c': {
        *p++ = (char)va_arg(args, int);
        break;
      }
      case '%': {
        *p++ = '%';
        break;
      }
      default:
        // Unsupported format specifier
        *p++ = '?';
        break;
    }
  }

  *p = '\0';
  va_end(args);
  return p - out;  // Return number of characters written
}
*/

int snprintf(char *out, size_t n, const char *fmt, ...) {
	va_list args;
    va_start(args, fmt);
    int ret = vsnprintf_internal(out, n, fmt, args);
    va_end(args);
    return ret;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    return vsnprintf_internal(out, n, fmt, ap);
}

#endif
