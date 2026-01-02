
#ifndef _XTRINGLIB_C
#define _XTRINGLIB_C



#include <stdarg.h>
//#include <stdlib.h>
//#include <string.h>
#include <ctype.h>
#include <math.h>


int xputs(int c, char **where);

int xpadit(int n, int c, int (*put)(), char **where);

int xprint(int (*put)(), char **where, char *fmt, va_list varg);
int pprint(int (*put)(), char **where, char *fmt, va_list varg);


int  xuxtoi(char *str);
long xuxtol(char *str);
int xatoi(char *str);
long xatol(char *str);

char *xultoa(unsigned long val, int base, int convup);
char *xfdtoa(double fval, int prec);
char *xftoa(float fval, int prec);

int xsprintf(char *where, char *fmt, ...);
int psprintf(char *where, char *fmt, ...);



/**************************************************************************/


#define CHRSTR          0       /* type is char or string*/
#define DOUX            1       /* type is d, o, u, x, X*/
#define CONVE           2       /* type is e, E*/
#define CONVF           3       /* type is f*/
#define CONVG           4       /* type is g,G*/
#define SIGN_OK         0       /* Signed*/
#define UNSIGN_OK       4       /* Unsigned*/

/***********/
int xputs(int c, char **where)
{
  return(*(*where)++ = c);
}
/***********/

/* Put out n characters.  No characters will be output if n <= 0.*/
int xpadit(int n, int c, int (*put)(), char **where)
{
  while(n-- > 0)
  {
    if ((*put)(c, where) != c) 
    {
      return 1; 
    }
  }
  return 0;
}
/***********/

char *xsltoa(long val, int base, int convup)
{
  static char buf[32];
  char sign = '+';
  char *p;
  char *str;

  if (val < 0) 
  { 
    val = -val;
    sign = '-';
  }

  str = convup?"0123456789ABCDEF":"0123456789abcdef";

  p = buf+31;
  *p = 0;

  do
  { 
    *--p = str[val % base];
  } while(val /= base);

  *--p = sign;
  return p;
}

/***********/

char *xultoa(unsigned long val, int base, int convup)
{
  static char buf[32];
  char *p;
  char *str;

  str = convup?"0123456789ABCDEF":"0123456789abcdef";

  p = buf+31;
  *p = 0;

  do
  {
    *--p = str[val % base];
  } while(val /= base);

  return p;
}
/**********************/

char *xfdtoa(double fval, int prec)
{ 

double rem, integ , _fval;
unsigned long	val , _val ;
static char buf[32];
char *p;
char *str;
double _pow;

	str = "0123456789ABCDEF";

	p = buf+31;
	*p = 0;	

	if(prec>0){

		rem = modf (fabs(fval),&integ);

		_pow = powf(10.0 , (double)prec);

		val = (unsigned long)(rem * _pow + 0.5); 
		_val = val;
		
		do
		{
			*--p = str[val % 10];
			val /= 10;
		} while(--prec>0);
		
		*--p = '.';

		val = (unsigned long)integ + _val/_pow; 

	}
	else
		val = ((unsigned long)(fabs(fval)*10 + 5))/10 ; 

	
	do
	{
	*--p = str[val % 10];
	}  while(val /= 10);

	
	
	if(fval<0)
		*--p ='-';
	else if(fval>0)
		*--p ='+';	
	
  return p;
}


char *xftoa(float fval, int prec)
{ float rem, integ;
  unsigned long	val;
  static char buf[32];
  char *p;
  char *str;

  str = "0123456789ABCDEF";  
  p = buf+31;
  *p = 0; 
  	
	rem = modff (fabsf(fval),&integ);

	if(prec>0){
		val = (unsigned long)(rem * ( (float)powf(10.0 , (double)prec))+0.5); 
	
		do
		{
			*--p = str[val % 10];
		} while(val /= 10);

		*--p = '.';

	}

	val = (unsigned long)integ; 
	
	do
	{
		*--p = str[val % 10];
	}  while(val /= 10);

	if(fval<0)
		*--p ='-';
	
  return p;
}


/***********/
int xprint(int (*put)(), char **where, char *fmt, va_list varg)
{
  int  c;
  int  zeros, left, sign, blank, alternate, longarg;
  int  width, precision, len, pad, prpad, prtsign, numalt, prttype;
  int  retv, getwidth, getprec, convup, padzero;
  long val;
  char *p;
  char buf[3];
  double fval;

  /*Valid printf characters (in specific order).*/
  static char validc[] = "douxXcsfeEgG";

  retv   = 0;
  buf[1] = 0;

  for (; (c=*fmt); ++fmt)
  {
      /* If the character's not a % or if it is a %% then just put out the character and continue.*/
      if ((c != '%') || ((c = *++fmt) == '%'))
      {
        ++retv;
        if ((*put)(c, where) != c)
        {
          return -1;
        }
        continue;
      }

      zeros = left = sign = blank = alternate = 0;

      do
      {
        if (c == '-')
          left = 1;
        else if (c == '+') 
          sign = 1;
        else if (c == ' ') 
          blank = 1;
        else if (c == '#') 
          alternate = 1;
        else if (c == '0') 
          zeros = 1;
        else 
          break;
      } 
      while (c = *++fmt);

      width = -1;
      if (getwidth = (c == '*'))
      {
        c = *++fmt;
      }
      else if (isdigit(c)) 
      {
        width = xatoi(fmt);
        while (c = *++fmt, isdigit(c))
        {
        }
      }

      getprec   = 0;
      precision = -1;
      if (c == '.')
      {
        c = *++fmt;
        if (getprec = (c == '*')) 
        {
          c = *++fmt;
        }
        else 
        {
          precision = xatoi(fmt);
          while(isdigit(c))
          { 
            c = *++fmt;
          }
        }
      }

      if (longarg = (c == 'l'))
      { 
        c = *++fmt;
      }

      padzero = 1;
      convup = numalt = prtsign = 0;

      if (p = strchr(validc, c))
      {
        if (getwidth)
        {
          width = va_arg(varg, int);
        }

        if (getprec)
        {
          precision = va_arg(varg, int);
        }		

		if(c== 'f'){
			fval =(float) va_arg(varg, double);
		}
        else if (p == (validc + SIGN_OK) )
        {
          val = (longarg?va_arg(varg, long):(long)va_arg(varg, int));
        }
        else if (p <= (validc + UNSIGN_OK))
        {
          val = (longarg?va_arg(varg, unsigned long):(long)va_arg(varg, unsigned int));
        }
      }

      switch(c)
      {
        case 'f':
            p = xfdtoa(fval, precision);
			prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
            break;
        case 'd':
            p = xsltoa(val, 10, 0);
            prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
            break;
		 
        case 'o':
            p = xultoa(val, 8, 0);
            numalt = 1;
            prttype = DOUX;
            break;
        case 'u':
            p = xultoa(val, 10, 0);
            prttype = DOUX;
            break;
        case 'X':
            convup = 1;
            /*fall thru*/
        case 'x':
            p         = xultoa(val, 16, convup);
            numalt    = 2;
            prttype   = DOUX;
            break;
        case 's':
            p         = va_arg(varg, char*);
            prttype   = CHRSTR;
            break;
        case 'c':
            c         = va_arg(varg, int);
            c         &= 0x00ff;
            /*fall thru*/
        default:
            buf[0]    = c;
            p         = buf;
            precision = 1;
            prttype   = CHRSTR;
      }



	 /*Handle the final printing of conversions for f*/
      if (prttype == CONVF) 
      {
	  	 p = xfdtoa(fval, precision);
            prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
      }
    /*Handle the final printing of conversions for d,o,u,x,X*/
      if (prttype == DOUX) 
      {
        if (val == 0)
        {
          alternate = 0;
          if (precision == 0)
          {
            *p = 0; 
          }
        }

        len = strlen(p);

        if ((prpad = precision - len) > 0)
        {
          if (alternate && (numalt == 1))
          { 
            --prpad;
          }
        }
        else
        { 
          prpad = 0;
        }

        pad = width - len - prpad;

        if (alternate)
        { 
          pad -= numalt;
        }

        if (prtsign)
        {
          --pad;
        }

        if (pad < 0)
        {
           pad = 0;
        }

        retv += len + pad + prpad;

        if (zeros)
        {
          if (!left && xpadit(pad, '0', put, where)) 
          {
            return -1;
          }
        }
        else 
        {
          if (!left && xpadit(pad, ' ', put, where)) 
          {
            return -1;
          }
        }

        if (prtsign) 
        {
          if ((*put)( prtsign, where ) != prtsign)
          {
            return -1;
          }
          ++retv;
        }

        if (alternate && numalt)
        {
          if ((*put)('0', where) != '0')
          {
            return -1;
          }
          retv += numalt;
          if ((numalt == 2) && ((*put)(c, where) != c)) 
          {
            return -1;
          }
        }

        if (xpadit(prpad, '0', put, where))
        {
          return -1;
        }
        while (c = *p++)
        { 
          if ((*put)(c, where) != c)
          {
            return -1; 
          }
        }
        if (left && xpadit(pad, ' ', put, where))
        {
          return -1; 
        }
      }
      else if (prttype == CHRSTR)
      {
      	if(p)
        	len = strlen(p);
		else
			len = 0;
		
        if ((precision != -1) && (precision < len)) 
        {
          len = precision;
        }

        if ((pad = width - len) < 0) 
        {
          pad = 0;
        }

        retv += width + pad;

        if (!left && xpadit(pad, ' ', put, where))
        { 
          return -1;
        }

        while (width-- && (c = *p++)) 
        {
          if ((*put)(c, where) != c)
          {
            return -1;
          }
        }
        if (left && xpadit(pad, ' ', put, where))
        {
          return -1;
        } 
      }
  }

  return retv;
}
/****************************/
int pprint(int (*put)(), char **where, char *fmt, va_list varg)
{
  int  c;
  int  zeros, left, sign, blank, alternate, longarg;
  int  width, precision, len, pad, prpad, prtsign, numalt, prttype;
  int  retv, getwidth, getprec, convup, padzero;
  long val;
  unsigned long *pval;
  char *p;
  char buf[3];
  double fval;
  float *pfval;
  unsigned long pointer;

  /*Valid printf characters (in specific order).*/
  static char validc[] = "douxXcsfeEgG";

  retv   = 0;
  buf[1] = 0;

  for (; (c=*fmt); ++fmt)
  {
      /* If the character's not a % or if it is a %% then just put out the character and continue.*/
      if ((c != '%') || ((c = *++fmt) == '%'))
      {
        ++retv;
        if ((*put)(c, where) != c)
        {
          return -1;
        }
        continue;
      }

      zeros = left = sign = blank = alternate = 0;

      do
      {
        if (c == '-')
          left = 1;
        else if (c == '+') 
          sign = 1;
        else if (c == ' ') 
          blank = 1;
        else if (c == '#') 
          alternate = 1;
        else if (c == '0') 
          zeros = 1;
        else 
          break;
      } 
      while (c = *++fmt);

      width = -1;
      if (getwidth = (c == '*'))
      {
        c = *++fmt;
      }
      else if (isdigit(c)) 
      {
        width = xatoi(fmt);
        while (c = *++fmt, isdigit(c))
        {
        }
      }

      getprec   = 0;
      precision = -1;
      if (c == '.')
      {
        c = *++fmt;
        if (getprec = (c == '*')) 
        {
          c = *++fmt;
        }
        else 
        {
          precision = xatoi(fmt);
          while(isdigit(c))
          { 
            c = *++fmt;
          }
        }
      }

      if (longarg = (c == 'l'))
      { 
        c = *++fmt;
      }

      padzero = 1;
      convup = numalt = prtsign = 0;

      if (p = strchr(validc, c))
      {
        if (getwidth)
        {
          width = va_arg(varg, int);
        }

        if (getprec)
        {
          precision = va_arg(varg, int);
        }	

		if(c!= 's'){
			
			pointer = va_arg(varg, unsigned long);

			if(!pointer)	
				return;	
		}
				
      }



      switch(c)
      {
        case 'f':

			pfval = pointer;

			p = xfdtoa(*pfval, precision);
			prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
            break;
        case 'd':

			pval = pointer;
				
            p = xsltoa(*pval, 10, 0);
            prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
            break;
		 
        case 'o':

			pval = pointer;
			
            p = xultoa(*pval, 8, 0);
            numalt = 1;
            prttype = DOUX;
            break;
        case 'u':

			pval = pointer;
			
            p = xultoa(*pval, 10, 0);
            prttype = DOUX;
            break;
        case 'X':
            convup = 1;
            /*fall thru*/
        case 'x':

			pval = pointer;
            p         = xultoa(*pval, 16, convup);
            numalt    = 2;
            prttype   = DOUX;
            break;
        case 's':
            p         = va_arg(varg, char*);
            prttype   = CHRSTR;
            break;
        case 'c':
            c         = va_arg(varg, int);
            c         &= 0x00ff;
            /*fall thru*/
        default:
            buf[0]    = c;
            p         = buf;
            precision = 1;
            prttype   = CHRSTR;
      }



	 /*Handle the final printing of conversions for f*/
      if (prttype == CONVF) 
      {
	  	 p = xfdtoa(fval, precision);
            prtsign = *p++;
            if (!sign && (prtsign == '+'))
            {
              prtsign = (blank?' ':0);
            }
            prttype = DOUX;
      }
    /*Handle the final printing of conversions for d,o,u,x,X*/
      if (prttype == DOUX) 
      {
        if (val == 0)
        {
          alternate = 0;
          if (precision == 0)
          {
            *p = 0; 
          }
        }

        len = strlen(p);

        if ((prpad = precision - len) > 0)
        {
          if (alternate && (numalt == 1))
          { 
            --prpad;
          }
        }
        else
        { 
          prpad = 0;
        }

        pad = width - len - prpad;

        if (alternate)
        { 
          pad -= numalt;
        }

        if (prtsign)
        {
          --pad;
        }

        if (pad < 0)
        {
           pad = 0;
        }

        retv += len + pad + prpad;

        if (zeros)
        {
          if (!left && xpadit(pad, '0', put, where)) 
          {
            return -1;
          }
        }
        else 
        {
          if (!left && xpadit(pad, ' ', put, where)) 
          {
            return -1;
          }
        }

        if (prtsign) 
        {
          if ((*put)( prtsign, where ) != prtsign)
          {
            return -1;
          }
          ++retv;
        }

        if (alternate && numalt)
        {
          if ((*put)('0', where) != '0')
          {
            return -1;
          }
          retv += numalt;
          if ((numalt == 2) && ((*put)(c, where) != c)) 
          {
            return -1;
          }
        }

        if (xpadit(prpad, '0', put, where))
        {
          return -1;
        }
        while (c = *p++)
        { 
          if ((*put)(c, where) != c)
          {
            return -1; 
          }
        }
        if (left && xpadit(pad, ' ', put, where))
        {
          return -1; 
        }
      }
      else if (prttype == CHRSTR)
      {
		if(p)
			len = strlen(p);
		else
			len = 0;
		
        if ((precision != -1) && (precision < len)) 
        {
          len = precision;
        }

        if ((pad = width - len) < 0) 
        {
          pad = 0;
        }

        retv += width + pad;

        if (!left && xpadit(pad, ' ', put, where))
        { 
          return -1;
        }

        while (width-- && (c = *p++)) 
        {
          if ((*put)(c, where) != c)
          {
            return -1;
          }
        }
        if (left && xpadit(pad, ' ', put, where))
        {
          return -1;
        } 
      }
  }

  return retv;
}



/***********/

int  xuxtoi(char *str)
{
  int  num;
  char c;

  c = *str;
  while ((c==' ') || (c=='\t'))
  {
    ++str;
  } 

  for(num=0; isxdigit(c); c=*++str)
  {
    if (c<='9') 
      c -= '0';
    else if (c<='F')
      c = (c-'A')+10;
    else  
      c = (c-'a')+10;

    num = (num*16) + (int)c;
  }

  return(num);
}

/***********/

long xuxtol(char *str)
{
  long num;
  char c;

  c = *str;
  while ((c==' ') || (c=='\t'))
  {
    ++str;
  } 

  for(num=0; isxdigit(c); c=*++str)
  {
    if (c<='9') 
      c -= '0';
    else if (c<='F')
      c = (c-'A')+10;
    else  
      c = (c-'a')+10;

    num = (num*16) + (long)c;
  }

  return(num);
}

/***********/

int xatoi(char *str)
{
  int  num;
  int  neg;
  char c;

  c = *str;

  while ((c == ' ') || (c == '\t'))
  {
    ++str;
  }

  if (*str == '-')
  {
    neg = 1;
    ++str;
  }
  else
  {
    neg = 0;
  }

  for(num=0; isdigit(c); c = *++str)
  {
    num = (num*10) + (c -= '0');
  }

  if (neg)
  {
    return(-num);
  }
  else
  {
    return(num);
  }
}

/***********/

long xatol(char *str)
{
  long num;
  int  neg;
  char c;

  c = *str;

  while ((c == ' ') || (c == '\t'))
  {
    ++str;
  }

  if (*str == '-')
  {
    neg = 1;
    ++str;
  }
  else
  {
    neg = 0;
  }

  for(num=0; isdigit(c); c = *++str)
  {
    num = (num*10) + (long)(c -= '0');
  }

  if (neg)
  {
    return(-num);
  }
  else
  {
    return(num);
  }
}
/************************************/
int xsprintf(char *where, char *fmt, ...)
{
  int     retv;
  va_list varg;
        
  va_start(varg, fmt);
  retv = xprint(xputs, &where, fmt, varg);
  *where = 0;
  va_end(varg);

  return retv;
}
/************************************/
int psprintf(char *where, char *fmt, ...)
{
  int     retv;
  va_list varg;
        
  va_start(varg, fmt);
  retv = pprint(xputs, &where, fmt, varg);
  *where = 0;
  va_end(varg);

  return retv;
}

#endif
