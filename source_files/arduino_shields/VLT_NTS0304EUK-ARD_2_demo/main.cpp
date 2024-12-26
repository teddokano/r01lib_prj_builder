#include	"r01lib.h"
#include	"ldo/NTS0304EUK_ARD_LDO.h"
#include	"misc/potentiometer/AD5161.h"

#define	I2C_SETTING

#ifdef I2C_SETTING
I2C			i2c( I2C_SDA, I2C_SCL );
AD5161_I2C	potentiometer( i2c );

#define	STR	"I2C"
#else
SPI			spi( D11, D12, D13, D10 );
AD5161_SPI	potentiometer( spi );
#define	STR	"SPI"
#endif  // I2C_SETTING

BusOut				board_setting( D5, A3, A2 );
Nts0304euk_Ard_LDO1	ldo1;
Nts0304euk_Ard_LDO2	ldo2;

int main( void )
{
	printf( "Hello, NTS0304EUK-ARD demo\r\n" );

#ifdef I2C_SETTING
	board_setting = 0b100;
#else
	board_setting = 0b001;
#endif  // I2C_SETTING

	float	va, vb;
	
	while ( true )
	{
		printf( "Voltage :   A-side[V]   B-side[V] (%s operation mode)\r\n", STR );

		for ( int v1 = 0; v1 < Nts0304euk_Ard_LDO::v1_variation; v1++ )
		{
			for ( int v2 = 0; v2 < Nts0304euk_Ard_LDO::v2_variation; v2++ )
			{
				va	= ldo1.voltage( v1 );
				vb	= ldo2.voltage( v2 );
				
				if ( va > vb )
					continue;

				ldo1 = v1;
				ldo2 = v2;

				printf( "              %4.2f        %4.2f\r\n", va, vb );
				wait( 0.8 );  // wait voltages settle


				for ( int i = 0; i < 256; i++ )
				{
					potentiometer	= i;
					wait( 0.01 );
				}
			}
		}
	}
}

