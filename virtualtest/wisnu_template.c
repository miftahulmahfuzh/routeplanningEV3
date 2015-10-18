#pragma config(StandardModel, "EV3_REMBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//Definitions
//#define 2t1 (float)100/255
//#define 1t2 (float)255/100

//Configuration
#define color_number 7 //Sesuaikan
#define color_tolerance 20

int color_lib[color_number][3]={
//  R   G   B  //Skala 0-100. kalau 0-255, tinggal kalikan 2t1. misal {  0,200*2t1,255*2t1}
	{100,100,100},	//0 Putih_Arena
	{  14,  14,  14},	//1 Hitam_Garis
	{  0,  0,100},	//2 Biru_Start
	{ 17, 87, 17},  //3 Ijo_Simpang
	{100, 16, 16},  //4 Merah_Ujung
	{100,  0,100},  //5 Pink_Buntu
	{  4, 72, 36}   //6 Ijo_Pudar
//	{   ,   ,   },  //Tambahin kalo perlu, dengan nama warna yang jelas.
	};

int check_color(){
	int res=-1; //if no color.
	int i=0;
	long red=0;
	long grn=0;
	long blu=0;

	getColorRGB(colorSensor,red,grn,blu);//Baca warna
  displayTextLine(1,"R: %d, G: %d, B: %d",red,grn,blu);
	while(res==-1 && i<color_number){
		if(red>color_lib[i][0]-color_tolerance && red<color_lib[i][0]+color_tolerance &&
			 grn>color_lib[i][1]-color_tolerance && grn<color_lib[i][1]+color_tolerance &&
			 blu>color_lib[i][2]-color_tolerance && blu<color_lib[i][2]+color_tolerance){
			 res=i;
			 break;
		}
		i++;
	}
	return res;
}

void cekGaris() {
	int a;
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	while(1) {
		a=check_color();
	}
}

task main()
{
  while(true)
  {
  	//cekGaris();
  	//motor[leftMotor] = 100;
  	//motor[rightMotor] = 100;
  	//sleep(3);
  	int a=check_color();
  	switch(a){
    case 0:	//Putih_Arena -> Belok Kanan
    				motor[leftMotor]  = 15;
      			motor[rightMotor] = 55;
      			break;
    case 1: //Hitam_Garis -> Belok Kiri
    				motor[leftMotor]  = 55;
      			motor[rightMotor] = 15;
      			break;
    case 3: //Ijo_Simpang -> diem
    				moveMotorTarget(leftMotor,180,50);
    				moveMotorTarget(rightMotor,-180,-50);
    				//motor[leftMotor] =0;
    				//motor[rightMotor] =0;
    				//sleep(1000);
    				while(getMotorMoving(leftMotor) && getMotorMoving(rightMotor))
    					sleep(1);
      			break;
    case 4: //Merah_ujung -> Balik kanan
    				motor[leftMotor] = 100;
    				motor[rightMotor] = -100;
    				sleep(500);
      			break;

     default:
     				//motor[leftMotor]  = 0;
      			//motor[rightMotor] = 0;
    }
  }
}
