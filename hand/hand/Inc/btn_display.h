#ifndef BTN_DISPALY_H_
#define BTN_DISPALY_H_





#define LED1 4
#define LED2 3
#define LED3 6



#define Sega    0x01
#define Segb    0x02
#define Segc    0x04
#define Segd    0x08
#define Sege    0x10
#define Segf    0x20
#define Segg    0x40
#define Segh    0x80


#define Num0    ~(Sega + Segb + Segc + Segd + Sege + Segf       )
#define Num1    ~(           Segb + Segc                                       )
#define Num2    ~(Sega + Segb + Segd + Sege + Segg)
#define Num3    ~(Sega + Segb + Segc + Segd               + Segg)
#define Num4    ~(           Segb + Segc               + Segf + Segg)
#define Num5    ~(Sega            + Segc + Segd + Segf + Segg)
#define Num6    ~(Sega            + Segc + Segd + Sege + Segf + Segg)
#define Num7    ~(Sega + Segb + Segc                            )
#define Num8    ~(Sega + Segb + Segc + Segd + Sege + Segf + Segg)
#define Num9    ~(Sega + Segb + Segc + Segd        + Segf + Segg)
#define Numblk  ~(0                                             )


#define E   ~(Sega + Segd + Sege + Segf + Segg )
#define R    ~(Sega + Segb + Segc + Sege + Segf + Segg )

#define S     ~(Sega + Segc + Segd + Segf + Segg )

#define _E   ~(Sega + Segb + Segc + Segd + Segg )


#define F_STAR 10
#define F_JIN  11

#define F_MAKE_CODE  2
#define F_CONTROL    0
#define F_CURRENT    1

#define F_UP         8
#define F_DOWN       9

#define F_ENTER      14
#define F_BACK       15
#define F_EXIT       16

void beep();
void beep_long();
void led(int index,int stat);
void led_flash(int index);
void btn_handle(void);
void sed_smg_number(int index, int num);
void sed_smg(int index, int num);







#endif


