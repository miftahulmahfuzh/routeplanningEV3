#pragma config(StandardModel, "EV3_REMBOT")
/////QUEUE//////
#define Head(Q) (Q).HEAD
#define Tail(Q) (Q).TAIL
#define InfoHead(Q) (Q).T[(Q).HEAD]
#define InfoTail(Q) (Q).T[(Q).TAIL]
#define maxEl(Q) (Q).MaxEl
#define Nil 0
/*** Definisi elemen dan address ***/
typedef int infotype;
typedef int address; /* indeks tabel */
/* *** Definisi Type Queue *** */
typedef struct {
	infotype T[10]; /* tabel penyimpan elemen */
	address HEAD; /*alamat penghapusan*/
	address TAIL; /*alamat penambahan*/
	int MaxEl; /*maksimum banyaknya elemen queue*/
} Queue;
int IsEmpty(Queue Q) {
	return(Head(Q)==Nil && Tail(Q)==Nil);
}
int IsFull(Queue Q) {
	int m = Head(Q) - Tail(Q);
	return((m==1) || (m==(-1*maxEl(Q)+1)));
}
int NBElmt (Queue Q)
{
	int n;
	if (IsEmpty(Q)) n = 0;
	else if (Head(Q) <= Tail(Q)) n = Tail(Q) - Head(Q) + 1;
	else n = Q.MaxEl - Tail(Q) + Head(Q) + 1;
	return n;
}
void CreateEmpty (Queue *Q)
{
	maxEl(*Q) = 10;
	Head(*Q) = Nil;
	Tail(*Q) = Nil;
}
void Add (Queue *Q,infotype X)
{
	if (IsEmpty(*Q))
	{
		Tail(*Q) = 1;
		Head(*Q) = 1;
	}
	else
	{
		Tail(*Q) += 1;
		if (Tail(*Q) == (*Q).MaxEl + 1) Tail(*Q) = 1;
	}
	InfoTail(*Q) = X;
}
void Del (Queue *Q,infotype *X)
{
	(*X) = InfoHead(*Q);
	if (Head(*Q) == Tail(*Q))	{
		Head(*Q) = Nil;
		Tail(*Q) = Nil;
	}
	else {
		Head(*Q)++;
		if (Head(*Q) == maxEl(*Q) + 1) Head(*Q) = 1;
	}
}
////////////////
//Definitions
//#define 2t1 (float)100/255
//#define 1t2 (float)255/100

//Configuration
#define color_number 7 //Sesuaikan
#define color_tolerance 20

int color_lib[color_number][3]={
//  R   G   B  //Skala 0-100. kalau 0-255, tinggal kalikan 2t1. misal {  0,200*2t1,255*2t1}
	{100,100,100},	//0 Putih_Arena
	{ 14, 14, 14},	//1 Hitam_Garis
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

////////////////
#define red 1
#define green 0
int searchSpot() {
//menelusuri garis hitam sampai menemukan "color"
	int hue;
	int colortemp;
	int threshold = 65;
	moveMotorTarget(leftMotor,360,100);
	moveMotorTarget(rightMotor,360,100);
	while(getMotorMoving(leftMotor)||getMotorMoving(rightMotor))
		sleep(1);
	while(1)
	{
		// sensor sees light:
		if(getColorReflected(colorSensor) < threshold)
		{
			// counter-steer right:
			motor[leftMotor]  = 55;
			motor[rightMotor] = 15;
		}
		// sensor sees dark:
		else
		{
			// counter-steer left:
			motor[leftMotor]  = 15;
			motor[rightMotor] = 55;
		}
		hue=getColorHue(colorSensor);
		if(hue==99 || hue==253) {
			colortemp=hue==99?green:red;
			break;
		}
	}
	return colortemp;
}

#define left 0
#define right 1
#define straight -1
#define first -2
void turn(int direction,int degree) {
//berbelok ke arah "direction", sebesar "degree"
	int rightpower=direction==left?0:30;
	moveMotorTarget(leftMotor,-degree,-rightpower);
	moveMotorTarget(rightMotor,degree,rightpower);
	while(getMotorMoving(leftMotor) || getMotorMoving(rightMotor)) {
		sleep(1);
	}
}

int checkLine(int dir) {
	int a,degmax=0;
	int incdegree=dir==right?10:-10;
	int rightpower=dir==right?-50:50;
	int rightdegree=dir==right?-10:10;
	while(degmax<2300) {
		a=check_color();
		switch(a) {
 		case 0:
 				moveMotorTarget(leftMotor,-rightdegree,-rightpower);
  			moveMotorTarget(rightMotor,rightdegree,rightpower);
  			while(getMotorMoving(leftMotor) && getMotorMoving(rightMotor))
    			sleep(1);
    		degmax+=incdegree;
    		break;
    case 1:
    		motor[leftMotor]=0;
    		motor[rightMotor]=100;
    		sleep(100);
    		return 1;
  	}
 	}
}

int cekLine(int a,int b) {return 1;}

void stepAhead(int degree) {
//maju degree derajat putaran roda
	moveMotorTarget(leftMotor,degree,50);
	moveMotorTarget(rightMotor,degree,50);
	while(getMotorMoving(leftMotor)||getMotorMoving(rightMotor)) {
		sleep(1);
	}
}

void cekSimpang(Queue *Q) {
	if(cekLine(right,90)) {
		Add(Q,right);
		motor[leftMotor] = 0;
		motor[rightMotor] = 0;
	}
	if(cekLine(left,-90)) {
		Add(Q,straight);
		stepAhead(360);
	}
	if(cekLine(left,-90))
		Add(Q,left);
	turn(right,90);
}

void BFS(int dirBack,Queue *Q) {
	int x,y,degree,color;
	if(IsEmpty(*Q)) {}
	else {
		Del(Q,&x);
		switch(dirBack) {
			case right:
				y=(x==straight)?right:straight;
				break;
			case straight:
				y=(x==right)?left:right;
				break;
			case left:
				y=(x==straight)?left:straight;
				break;
			default:
				y=x; //first
		}
		if(y!=straight) {
			stepAhead(220);
			degree=(y==right)?85:-80;
			turn(y,degree);
		}
		color = searchSpot();
		if(color==green) {
			Add(Q,x);
			cekSimpang(Q);
		}
		turn(-1,-180);
		BFS(x,Q);
	}
}

task main()
{
	Queue Q;
	CreateEmpty(&Q);
	int cek = checkLine(right);
	/*int color = searchSpot();
	if(color==green) {
		stepAhead(220);
		turn(right,20);
		cekSimpang(&Q);
	}
	BFS(first,&Q);*/
}
