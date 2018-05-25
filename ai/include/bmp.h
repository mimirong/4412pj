#ifndef	__BMP__
#define	__BMP__

#define	BI_RGB		0x00
#define	BI_RLE8		0x01
#define	BI_RLE4		0x02
#define	BI_BITFIELDS	0x03
#define	BI_JPEG		0x04
#define	BI_PNG		0x05

#define	BPP_1		1
#define	BPP_4		4
#define	BPP_8		8
#define	BPP_16		16
#define	BPP_24		24
#define	BPP_32		32

/*
  *	bf_type:
  *		BM	- windows 3.1x, 95, NT, ...
  *		BA	- OS/2 Bitmap Array
  *		CI	- OS/2 Color Icon
  *		CP	- OS/2 Color Pointer
  *		IC	- OS/2 Icon
  *		PT	- OS/2 Pointer
  */
#pragma pack(2)			// if not, struct will be aligned at 4
struct bmp_file_header
{
	unsigned short	bf_type;
	unsigned int	bf_size;
	unsigned short	rsvd1;
	unsigned short	rsvd2;
	unsigned int	offset;
};//__attribute__((packed));	// use it, align at 1

#pragma pack(2)
struct bmp_info_header
{
	unsigned int	header_size;	// size of struct
	unsigned int	width;
		 int	height;		// if > 0, its top-down dip
	unsigned short	planes;
	unsigned short	bpp;		// bits per pixel
	unsigned int	compression;
	unsigned int	image_size;
	unsigned int	xppm;		// pixel per meter
	unsigned int	yppm;
	unsigned int	color_used;
	unsigned int	color_important;
};//__attribute__((packed));

struct bmp_info
{
	struct bmp_file_header	file_head;
	struct bmp_info_header	info_head;
	unsigned int	*palette;
	unsigned char	*pdat;
};


#endif
