#include <AT898252.H>
#include <stdio.h>

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
#define AT250X0_WREN_OPCODE     (0x06)
#define AT250X0_WRDI_OPCODE     (0x04)
#define AT250X0_RDSR_OPCODE     (0x05)
#define AT250X0_WRSR_OPCODE     (0x01)
#define AT250X0_READ_OPCODE(a)  ((((a) & 0x0100) >> 0x05) | 0x03)
#define AT250X0_WRITE_OPCODE(a) ((((a) & 0x0100) >> 0x05) | 0x02)

#define AT250X0_ADDRESS_LSB(a)  ((a) & 0x00FF)

#define AT250X0_CS	P1_0

#define AT250X0_STAT_BUSY	0x01
#define AT250X0_STAT_WREN	0x02

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
unsigned char read_status (void)
{
	unsigned char stat;

	AT250X0_CS = 0;

	SPDR = AT250X0_RDSR_OPCODE;
	while ((SPSR & 0x80) == 0);

	SPDR = 0xFF;
	while ((SPSR & 0x80) == 0);
	stat = SPDR;

	AT250X0_CS = 1;

	return (stat);
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void write_enable (
  unsigned char flag)
{
	AT250X0_CS = 0;

	SPDR = (flag == 0) ? AT250X0_WRDI_OPCODE : AT250X0_WREN_OPCODE;
	while ((SPSR & 0x80) == 0);

	AT250X0_CS = 1;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void read_at250x0 (
	unsigned address,
	unsigned char *buf,
	unsigned length)
{
	AT250X0_CS = 0;

	SPDR = AT250X0_READ_OPCODE(address);
	while ((SPSR & 0x80) == 0);

	SPDR = AT250X0_ADDRESS_LSB(address);
	while ((SPSR & 0x80) == 0);

	while (length--)
	  {
	  SPDR = 0xFF;
	  while ((SPSR & 0x80) == 0);
	  *buf++ = SPDR;
	  }

	AT250X0_CS = 1;
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void write_at250x0 (
  unsigned address,
  unsigned char *buf,
  unsigned length)
{
	write_enable (1);

	for (; length--; address++)
	  {
	  while (read_status () & AT250X0_STAT_BUSY);

	  AT250X0_CS = 0;

	  SPDR = AT250X0_WRITE_OPCODE(address);
	  while ((SPSR & 0x80) == 0);

	  SPDR = AT250X0_ADDRESS_LSB(address);
	  while ((SPSR & 0x80) == 0);

	  SPDR = *buf++;
	  while ((SPSR & 0x80) == 0);

	  AT250X0_CS = 1;
	  }

	write_enable (0);
}

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/
void main (void)
{
	unsigned i;
	static unsigned char xdata buffer [20];

	/*-----------------------------------------------
	Initialize the serial port.
	-----------------------------------------------*/
	SCON  = 0x50;
	TMOD |= 0x20;
	TH1   = 221;
	TR1   = 1;
	TI    = 1;

	/*-----------------------------------------------
	-----------------------------------------------*/
	SPCR = 0x53;   /* 01010011 */

	/*-----------------------------------------------
	-----------------------------------------------*/
	while (1)
	  {
	  write_at250x0 (0, "This is a test", 15);
	  read_at250x0 (0, buffer, sizeof(buffer));

	  for (i = 0; i < sizeof(buffer); i++)
		printf ("Buffer[%u]=%2.2X\n", (unsigned) i, (unsigned) buffer[i]);
	  }

	/*-----------------------------------------------
	-----------------------------------------------*/
}

