#pragma config(StandardModel, "EV3_REMBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//Definitions
//#define 2t1 (float)100/255
//#define 1t2 (float)255/100

//Configuration
#define color_number 5 //Sesuaikan
#define color_tolerance 25

int color_lib[color_number][3]={
//  R   G   B  //Skala 0-100. kalau 0-255, tinggal kalikan 2t1. misal {  0,200*2t1,255*2t1}
	{100,100,100},	//0 Putih_Arena
	{  0,  0,  0},	//1 Hitam_Garis
	{  0,  0,100},	//2 Biru_Start
	{  0,100,  0},  //3 Ijo_Simpang
	{100,  0,  0},  //4 Merah_Ujung
	{100,  0,100},  //5 Pink_Buntu
//	{   ,   ,   },  //Tambahin kalo perlu, dengan nama warna yang jelas.
	};

int check_color(){
	int res=-1; //if no color.
	int i=0;
	long red=0;
	long grn=0;
	long blu=0;

	getColorRGB(colorSensor,red,grn,blu);//Baca warna
	while(res==-1 && i<color_number){
		if(red>color_lib[i][0]-color_tolerance && red<color_lib[i][0]+color_tolerance &&
			 grn>color_lib[i][1]-color_tolerance && grn<color_lib[i][1]+color_tolerance &&
			 blu>color_lib[i][2]-color_tolerance && blu<color_lib[i][2]+color_tolerance){
			 res=i;
		}
		i++;
	}
	return res;
}


task main()
{
  while(true)
  {
  	int a=check_color();
    switch(a){
    case 0:	//Putih_Arena -> Belok Kanan
    				motor[leftMotor]  = 40;
      			motor[rightMotor] = -20;
      			break;
    case 1: //Hitam_Garis -> Belok Kiri
    				motor[leftMotor]  = -20;
      			motor[rightMotor] = 40;
      			break;
    case 4: //Ijo_Simpang -> diem
    				motor[leftMotor]  = 0;
      			motor[rightMotor] = 0;
      			break;
    case 5: //Hitam_Garis -> Belok Kiri
    				motor[leftMotor]  = -20;
      			motor[rightMotor] = 40;
      			break;

     default:
     				//motor[leftMotor]  = 0;
      			//motor[rightMotor] = 0;
    }
  }
}