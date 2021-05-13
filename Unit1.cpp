#include <math.h>
#include <time.h>
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include<VclTee.TeeGDIPlus.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
const int n=20;
double IshodDan[n][n],Days[n];
double AlphaGL[n],MUGL[n];
double MUprog[n],ALprog[n],MUprog1=0.0,ALprog1=0.0,A,MS=0.0;
double MUm[n],MUp[n],E,MUpm[n],MUi[n];
double Mdek[n][n];
int i=0,j=0,Kolstrok,Kolstrolb,Stolb=1,Novstrok,Stolb1=1;
AnsiString constr; //Используется для подключения БД
AnsiString s1="Provider=Microsoft.Jet.OLEDB.4.0;Data Source="; //Используется для подключения БД
AnsiString s2=";Persist Security Info=False"; //Используется для подключения БД

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
		: TForm(Owner)
{
StringGrid1->Cells[0][0]="Days" ;
StringGrid1->Cells[1][0]="MUGL" ;
StringGrid1->Cells[2][0]="Alpha" ;
StringGrid2->Cells[0][0]="Days" ;
StringGrid3->Cells[0][0]="Days" ;
StringGrid4->Cells[0][0]="Days";
StringGrid4->Cells[1][0]="MU прогн" ;
StringGrid4->Cells[2][0]="Alpha прогн" ;
StringGrid5->Cells[0][0]="Days" ;
StringGrid5->Cells[1][0]="MUGL-" ;
StringGrid5->Cells[2][0]="MUGL" ;
StringGrid5->Cells[3][0]="MUGL+" ;
StringGrid5->Cells[4][0]="|MUGL+ - MUGL-|" ;
StringGrid5->Cells[5][0]="|MUi-MUo|" ;
StringGrid5->Cells[6][0]="Устойчивость" ;
StringGrid6->Cells[0][0]="Days" ;
StringGrid6->Cells[1][0]="MUGL" ;
StringGrid6->Cells[2][0]="Alpha" ;
StringGrid6->Cells[3][0]="MU прогн" ;
StringGrid6->Cells[4][0]="Alpha прогн" ;
StringGrid6->Cells[5][0]="Устойчивость" ;
StringGrid8->Cells[0][0]="Days" ;
StringGrid9->Cells[0][0]="Days" ;
StringGrid13->Cells[0][0]="Days" ;
StringGrid13->Cells[1][0]="MUGL" ;
StringGrid13->Cells[2][0]="Alpha" ;
StringGrid13->Cells[3][0]="MU прогн" ;
StringGrid13->Cells[4][0]="Alpha прогн" ;
StringGrid13->Cells[5][0]="Устойчивость" ;
RichEdit1->Lines->Clear();
RichEdit1->Lines->LoadFromFile("Инструкция.rtf");
RichEdit1->Visible=true;
RichEdit2->Lines->Clear();
RichEdit2->Lines->LoadFromFile("Глоссарий.rtf");
RichEdit2->Visible=true;
UnicodeString Site=ExtractFilePath(Application->ExeName);
CppWebBrowser1->Silent = true;
CppWebBrowser2->Silent = true;
TVariant x=Site+"варианты.htm";
CppWebBrowser1->Navigate2(&x,NULL,NULL,NULL,NULL);
x=Site+"Задание на курсовую работу по дисциплине.htm";
CppWebBrowser2->Navigate2(&x,NULL,NULL,NULL,NULL);

WideString M1=ExtractFilePath(Application->ExeName);
MediaPlayer1->FileName=(M1+"Инструкция1.avi");
MediaPlayer1->Open();
MediaPlayer1->DisplayRect;


WideString M2=ExtractFilePath(Application->ExeName);
MediaPlayer2->FileName=(M2+"Инструкция2.avi");
MediaPlayer2->Open();
MediaPlayer2->DisplayRect;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
OpenDialog1->Execute(); //Открытие OpenDialog1
if (ADOTable1->Active) ADOTable1->Active=false; //Если ADOTable1 включен, выключить
if (ADOConnection1->Connected==true) ADOConnection1->Connected=false; //Если ADOConnection1 подключено, выключить
ComboBox1->Clear(); //Очистить ComboBox1
if (OpenDialog1->FileName!="") //Если имя файла не пустое
   {
   Label10->Caption=OpenDialog1->FileName; //В Label10 записать название файла
   constr=s1+Label10->Caption+s2; //Присваивание переменной информации для подключения БД
   ADOConnection1->ConnectionString=constr; //Присвоение ADOConnection1 информации для подключения БД
   ADOConnection1->Connected=true; //Включение ADOConnection1
   ADOConnection1->GetTableNames(ComboBox1->Items); //Передача названия имя таблицы из ComboBox1
}
}
//---------------------------------------------------------------------------

double sokr(double x1,double y1)
{
int z1 =(int)(x1*y1);
x1=z1/y1;
return x1; //Возвращаем значение x1
}
//---------------------------------------------------------------------------

double  TForm1::Perevod_AlphaGL(double A, int mtype)
{
if (mtype==1)
		{
		A=A*57,2958; //Перевод в градусы
		}
		else if (mtype==2)
		{
		A=A*3437,747; //Перевод в секунды
		}
		else if (mtype==3)
		{
		A=A*206264,806; //Перевод в секунды
		}
return A;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::raschet_MUGL_i_AlphaGL() //Функция расчёта Мю и Альфа массивов
{
double AL;
for(i=0;i<Kolstrok;i++) //До тех пор, пока не равно кол-ву строк
		{
		double t=0;
		for(j=0; j<Kolstrolb; j++) //До тех пор, пока не равно кол-ву столбцов
			t+=IshodDan[j][i]*IshodDan[j][i]; //Сумма квадратов высот исходных дынных, t = h1*h1+h2*h2...
		MUGL[i]= sqrt(t); //Длина вектора Мю равен корню из суммы квадратов высот исходных данных, MU=корень(t)
		t=0;
		for(j=0; j<Kolstrolb; j++)
			t+=IshodDan[j][i]*IshodDan[j][0]; //Сумма квадратов высот исходных дынных, t = h1*h1+h2*h2...
		AL=t/fabs(MUGL[i]*MUGL[0]); //Произведение высот деленное на модуль произведения высот
		if(AL>1) //Так как косинус не может быть больше 1
		AL=1;
		AlphaGL[i]=Perevod_AlphaGL(sokr(acos(AL),10000000),Form4->ComboBox2->ItemIndex); //Угол между векторами. arccos(AL)
		}
}
//---------------------------------------------------------------------------

 void __fastcall TForm1::raschet_MU_prog() //Расчет прогнозного значения Мю и Альфа
{
for(i=1; i<=Kolstrok; i++)
	MS=MUGL[i-1]+MS; //Складывает все значения МЮ
MS=MS/Kolstrok; //Находит среднее значение МЮ
A=StrToFloat(Edit2->Text); //Считывает значение коэффициента
MUprog1=A*MUGL[0]+(1-A)*MS; //Коэф умноженный на нулевой значение МЮ плюс Среднее занчение умноженной на 1 минус коэф
MUprog[0]=MUprog1; //Приравнивание
for(i=1;i<Kolstrok;i++)
	MUprog[i]=A*MUGL[i]+(1-A)*MUprog[i-1]; //Коэф умноженный на нулевой значение МЮ плюс Среднее занчение умноженной на 1 минус коэф
MUprog[Kolstrok]=A*MS+(1-A)*MUprog[Kolstrok-1];
MS=0;
for(i=1;i<Kolstrok;i++)
	MS=AlphaGL[i-1]+MS;
MS=MS/Kolstrok;
ALprog1=A*AlphaGL[0]+(1-A)*MS; //Коэф умноженный на нулевой значение АЛЬФА плюс Среднее занчение умноженной на 1 минус коэф
ALprog[0]=ALprog1;
for(i=1;i<Kolstrok;i++)
	ALprog[i]=A*AlphaGL[i]+(1-A)*ALprog[i-1];
ALprog[Kolstrok]=A*MS+(1-A)*ALprog[Kolstrok-1];
MS=0;
}
//----------------------------------------------------------------------------

void __fastcall TForm1::raschet_ustoi() //Расчёт устойчивости функции
{
E=StrToFloat(Edit3->Text);
for (i=0; i<Kolstrok; i++)
	   {double t=0;
	   for(j=0;j<Kolstrolb;j++) //Пока не пройдется по всем столбцам
	   t+=(IshodDan[j][i]-E)*(IshodDan[j][i]-E); //Сумма квадратов высот исходных дынных, минус погрешность. t = (h1-E)*(h1-E)+(h2-E)*(h2-E)...
	   MUm[i]= sqrt(t); //Длина вектора, с погрешностью. Корень из суммы квадратов исходных данных минус погрешность.MUm=корень t
	   t=0; //Обнуляем t
	   }
for (i=0; i<Kolstrok; i++)
	   {double t=0;
	   for(j=0;j<Kolstrolb;j++)
	   t+=(IshodDan[j][i]+E)*(IshodDan[j][i]+E); //Сумма квадратов высот исходных дынных, плюс погрешность. t = (h1+E)*(h1+E)+(h2+E)*(h2+E)...
	   MUp[i]= sqrt(t); //Длина вектора, с погрешностью. Корень из суммы квадратов исходных данных плюс погрешность.MUm=корень t
	   t=0; //Обнуляем t
	   }
for (i=0;i<Kolstrok;i++)
MUpm[i]=MUp[i]-MUm[i]; //Разность. исп для неравенства, которое необходимо для определения устойчивости.
for( i=0;i<Kolstrok;i++)
MUi[i]=MUGL[i]-MUGL[0]; //Разность между первым и последующими значениями МЮ. исп для неравенства, которое необходимо для определения устойчивости.
}
//--------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
if (ADOTable1->Active==true) ADOTable1->Active=false; //Если ADOTable1 активен, выключить
if (ComboBox1->Text!="") //Если текст в ComboBox1 присутствует
{
Image1->Picture->LoadFromFile(""+OpenDialog1->FileName+".bmp"); //Загрузка изображения варианта
ADOTable1->TableName=ComboBox1->Text;
ADOTable1->Active=true;
Kolstrolb=ADOTable1->FieldCount-1; //Количество столбцов - 1
Kolstrok=ADOTable1->RecordCount; //Количество строк
Novstrok=Kolstrok; //Новая строка равна количеству строк
StringGrid7->ColCount=Kolstrolb; //Добавление столбца
StringGrid3->ColCount=Kolstrolb+1; //Добавление столбца
StringGrid3->RowCount=Kolstrok+1; //Добавление строки
StringGrid1->RowCount=Kolstrok+1; //Добавление строки
StringGrid4->RowCount=Kolstrok+2; //Добавление двух строк
StringGrid5->RowCount=Kolstrok+1; //Добавление строки
StringGrid2->RowCount=Kolstrok+1; //Добавление строки
StringGrid6->RowCount=Kolstrok+1; //Добавление строки
StringGrid9->RowCount=Kolstrok+1; //Добавление строки
StringGrid8->RowCount=Kolstrok+1; //Добавление строки
StringGrid13->RowCount=Kolstrok+1; //Добавление строки
CheckListBox1->Clear();
for(int i=1;i<Kolstrolb+1;i++)  //Цикл до тех пор, пока не станет кол-ву столбцов + 1
	 {
	 CheckListBox1->Items->Append(IntToStr(i));
	 StringGrid3->Cells[i][0]=i;    //Нумерация стобцов от 1 до тех пор, пока не станет
	 StringGrid7->Cells[i-1][0]=i;  //равным количеству столбцов в таблице БД
	 }
for (i=1;i<Kolstrok+1;i++ )  //Цикл до тех пор, пока не станет кол-ву строк
	 {DBGrid1->DataSource->DataSet->RecNo = i; //Номер текущей записи равен i
	 Days[i-1]=DBGrid1->Fields[0]->AsFloat;  //Текст отдельной ячейки равен дню из БД
	 StringGrid1->Cells[0][i]= Days[i-1]; //
	 StringGrid3->Cells[0][i]= Days[i-1]; //
	 StringGrid4->Cells[0][i]= Days[i-1]; //Запись
	 StringGrid2->Cells[0][i]= Days[i-1]; //номеров
	 StringGrid5->Cells[0][i]= Days[i-1]; //дней
	 StringGrid6->Cells[0][i]= Days[i-1]; //в
	 StringGrid8->Cells[0][i]= Days[i-1]; //таблицы
	 StringGrid9->Cells[0][i]= Days[i-1]; //
	 StringGrid13->Cells[0][i]= Days[i-1]; //
	 }
for (i=0;i<Kolstrolb;i++ )
for ( j=0;j<Kolstrok;j++ )
	 {   DBGrid1->DataSource->DataSet->RecNo = j+1; //Номер текущей записи равен j+1
	 IshodDan[i][j]=DBGrid1->Fields[i+1]->AsFloat; //Массив исходных данных равен тексту отдельной ячейки из БД
	 StringGrid3->Cells[i+1][j+1]=FloatToStr(IshodDan[i][j]); //Запись исходных данных в таблицу "Загрузка данных"
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
double B,C,C1,C3;
double k;
for (i=0;i<Kolstrolb;i++ )
for (j=0;j<Kolstrok;j++ )
	{DBGrid1->DataSource->DataSet->RecNo = j+1;
	IshodDan[i][j]=DBGrid1->Fields[i+1]->AsFloat;
	}
srand(time(NULL));
Kolstrok++;
StringGrid3->RowCount= Kolstrok+1;
StringGrid1->RowCount= Kolstrok+1;
StringGrid2->RowCount= Kolstrok+1;
StringGrid4->RowCount= Kolstrok+2;
StringGrid6->RowCount= Kolstrok+1;
StringGrid5->RowCount= Kolstrok+1;
StringGrid8->RowCount= Kolstrok+1;
StringGrid9->RowCount= Kolstrok+1;
for ( j=0;j<Kolstrolb;j++)
	  {   C=0;
	  for( i=0;i<Novstrok;i++)
	  {
	  B=fabs(IshodDan[j][i]-IshodDan[j][0]) ;
	  if(B>C) C=B;
	  }
	  k=0;
	  while(C<10)
	  { C=C*10;
	  k=k+1;
	  }
	  C1=rand()%((int)(C*2+1-C))/pow(10,k);
	  IshodDan[j][Kolstrok-1]= C1+IshodDan[j][0];
	  }
Days[Kolstrok-1]= Days[Kolstrok-2]+0.03;
StringGrid3->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid1->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid4->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid6->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid2->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid5->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid8->Cells[0][Kolstrok]=Days[Kolstrok-1];
StringGrid9->Cells[0][Kolstrok]=Days[Kolstrok-1];
for ( j=0;j<Kolstrolb;j++)
StringGrid3->Cells[j+1][Kolstrok]=FloatToStr(IshodDan[j][Kolstrok-1]);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
{
for (i=0; i<Kolstrolb; i++)
	for (j=0; j<Kolstrok; j++)
		{
		DBGrid1->DataSource->DataSet->RecNo = j+1;
		IshodDan[i][j]=DBGrid1->Fields[i+1]->AsFloat;
		}
raschet_MUGL_i_AlphaGL(); //Расчет Мю и Альфа
for(i=0;i<Kolstrok;i++)
	{
	StringGrid1->Cells[1][i+1]=FloatToStr(MUGL[i]); //Запись значений МЮ в ячейки (1; 0), (1; i+1) в StringGrid1
	StringGrid1->Cells[2][i+1] = FloatToStrF(AlphaGL[i],ffFixed,9,9); //Запись значений АЛЬФА в ячейки (2; 0), (2; i+1) в StringGrid1
	}
raschet_MU_prog(); //Расчет прогнозных Мю и Альфа
for(i=0; i<Kolstrok+1; i++)
	StringGrid4->Cells[1][i+1]=FloatToStr(MUprog[i]); //Запись значений МЮ в ячейки (1; 0), (1; i+1) в StringGrid4
for(i=0; i<Kolstrok+1; i++)
	StringGrid4->Cells[2][i+1] = FloatToStrF(ALprog[i],ffFixed,9,9); //Запись значений АЛЬФА в ячейки (2; 0), (2; i+1) в StringGrid4
StringGrid4->Cells[0][Kolstrok+1]="Прогноз";
double x,y,x1,y1,a,b,a1,b1;
Series1->Clear();//
Series2->Clear();//Очищение
Series9->Clear();//Series
Series10->Clear();//
for(int i=0; i<Kolstrok; i++)
	{
	x=MUGL[i]; //МЮ
	y=AlphaGL[i]; //Альфа
	a=MUprog[i]; //МЮ прогнозное
	b=ALprog[i]; //АЛЬФА прогнозное
	Series1->AddXY(x,y); //Создание графика (линии) МЮ
	Series9->AddXY(x,y); //Создание графика(точек) МЮ
	Series2->AddXY(a,b); //Создание графика(линии) МЮ прогноз
	Series10->AddXY(a,b); //Создание графика(точек) МЮ прогнозное
	}
Chart1->Refresh();
}
for (i=0;i<Kolstrolb;i++ )
	for ( j=0;j<Kolstrok;j++ )
		{
		DBGrid1->DataSource->DataSet->RecNo = j+1;    //Переход
		IshodDan[i][j]=DBGrid1->Fields[i+1]->AsFloat; //по строкам
		}
raschet_ustoi(); //Расчёт устойчивости
for(i=0; i<Kolstrok; i++)
	{
	StringGrid5->Cells[2][i+1]=FloatToStr(MUGL[i]); //Запись значений МЮ в ячейки (2; 0), (2; i+1) в StringGrid5
	}
	for( i=0;i<Kolstrok;i++)
		{
		StringGrid5->Cells[1][i+1]=FloatToStr(MUm[i]); //Запись значений МЮ минус E (погрешность)
		StringGrid5->Cells[3][i+1]=FloatToStr(MUp[i]); //Запись значений МЮ плюс E (погрешность)
		StringGrid5->Cells[4][i+1]=FloatToStr(MUpm[i]); //Запись значений разности значений (МЮ плюс E - МЮ минус E)
		StringGrid5->Cells[5][i+1]=FloatToStr(MUi[i]); //Запись значений разности между первым и последующими значениями МЮ
		}
	for( i=0;i<Kolstrok;i++)
		if(MUpm[i]>=MUi[i]) //Если (разность значений (МЮ плюс E - МЮ минус E)) больше (разности между первым и последующими значениями МЮ)
		StringGrid5->Cells[6][i+1]="+"; //Запись положительного значения
		else
		StringGrid5->Cells[6][i+1]="-"; //Запись отрицательного значения
double x=0,y=0,a=0,b=0;
Series3->Clear(); //
Series4->Clear(); //
Series5->Clear(); //Очищение
Series6->Clear(); //Series
Series7->Clear(); //
Series8->Clear(); //
for(int i=0; i<Kolstrok-1; i++)
	{
	x=MUm[i]; //МЮ минус погрешность E
	y=AlphaGL[i]; //Альфа
	a=MUGL[i]; //МЮ
	b=MUp[i]; //МЮ плюс погрешность E
	Series3->AddXY(x,y); //Создание графика(линии) МЮ минус
	Series4->AddXY(a,y); //Создание графика(линии) МЮ
	Series5->AddXY(b,y); //Создание графика(линии) МЮ плюс
	Series6->AddXY(x,y); //Создание графика (точек) МЮ минус
	Series7->AddXY(a,y); //Создание графика (точек) МЮ
	Series8->AddXY(b,y); //Создание графика (точек) МЮ плюс
	}
	Chart2->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid3SelectCell(TObject *Sender, int ACol,
int ARow, bool &CanSelect)
{
AnsiString s1,s2;
int k=1;
s1=StringGrid7->Cells[ACol][0]; //Число находящее в ячейке
if(Stolb<2)
	{StringGrid2->ColCount= Stolb+1;
	for(int i=0; i<Kolstrok+1; i++)
		{
		StringGrid2->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
		}
	Stolb++;
	}
else
{
for(j=0;j<Stolb-1;j++)
	{
	s2= StringGrid2->Cells[j+1][0];
	if(s1==s2) k=k*0;
	}
if(k==1)
	{ StringGrid2->ColCount= Stolb+1;
	for(int i=0;i<Kolstrok+1;i++)
	{
	StringGrid2->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
	}
	Stolb++;
	}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
StringGrid2->ColCount=1;
Stolb=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
for(j=0;j<Kolstrolb;j++)
for(i=0;i<Kolstrok;i++)
IshodDan[j][i]=0;
for(j=0;j<Stolb-1;j++)
for(i=0;i<Kolstrok;i++)
IshodDan[j][i]=StrToFloat(StringGrid2->Cells[j+1][i+1]);
raschet_MUGL_i_AlphaGL();
raschet_MU_prog();
raschet_ustoi();
for(i=0;i<Kolstrok;i++)
	   {
	   StringGrid6->Cells[1][i+1]=FloatToStr(MUGL[i]);
	   StringGrid6->Cells[2][i+1] = FloatToStrF(AlphaGL[i]*10.0,ffFixed,9,9);
	   }

 for(i=0;i<=Kolstrok;i++)
 StringGrid6->Cells[3][i+1]=FloatToStr(MUprog[i]);
 for(i=0;i<=Kolstrok;i++)
 {
 StringGrid6->Cells[4][i+1] = "0,0" + FloatToStr(ALprog[i]*10.0).SubString(3,9);
 }
 StringGrid6->Cells[0][Kolstrok+1]="Прог" ;

 for( i=0;i<Kolstrok+1;i++)
 if(MUpm[i]>=MUi[i])
 StringGrid6->Cells[5][i+1]="+";
 else StringGrid6->Cells[5][i+1]="-";

double x,y,a,b,a1,b1,x1,y1;
Series11->Clear();
Series12->Clear();
Series13->Clear();
Series14->Clear();
for(int i=0; i<Kolstrok-1; i++)
	{
	x=MUGL[i];
	y=AlphaGL[i];
	a=MUprog[i];
	b=ALprog[i];
	Series11->AddXY(x,y);
	Series12->AddXY(a,b);
	Series13->AddXY(x,y);
	Series14->AddXY(a,b);
	}
Chart3->Refresh();
//=================
for(j=0;j<Stolb-1;j++)
StringGrid11->Cells[j][0]=StringGrid2->Cells[j+1][0] ;
StringGrid11->ColCount=Stolb-1;

int q1,q2;
int w=1;

for (int i=0;i<StringGrid11->ColCount-1;i++) //По кол-во столбцов - 1
	for (int k=i+1;k<StringGrid11->ColCount;k++) //По кол-во столбцов - 1
		{
		q1=StrToInt(StringGrid11->Cells[i][0]);//Значение в столбце
		q2=StrToInt(StringGrid11->Cells[k][0]); //Слебующее значение в столбце
		StringGrid9->Cells[w][0]=StringGrid11->Cells[i][0]+"-"+StringGrid11->Cells[k][0]; //Подпись столбцов
		for (int t=1;t<Kolstrok+1;t++) //Пока не дойдет до последней строчки
			{
			StringGrid9->Cells[w][t]=FloatToStr(fabs(StrToFloat(StringGrid3->Cells[q1][t])-StrToFloat(StringGrid3->Cells[q2][t])));
			}
		 StringGrid9->ColCount=StringGrid9->ColCount+1;
		 w++;
		}
StringGrid9->ColCount=StringGrid9->ColCount-1;

Stolb=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid2Click(TObject *Sender)
{
if(StringGrid2->Col!=0 && Stolb>1)
	{
	StringGrid2->Cols[StringGrid2->Col]->Clear();
	if(StringGrid2->Col+1<Stolb)
		{
		for(i=0;i<Kolstrok+1;i++)
			for(j=StringGrid2->Col+1;j<=Stolb;j++)
				StringGrid2->Cells[j-1][i]=StringGrid2->Cells[j][i];
		}
	Stolb--;
	StringGrid2->ColCount--;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid9DrawCell(TObject *Sender, int ACol,
int ARow, TRect &Rect, TGridDrawState State)
{
if (StringGrid9->Cells[ACol][ARow]!="" && ACol>0 && ARow>0)
{
if (StrToFloat(StringGrid9->Cells[ACol][ARow])>E*2)
{
StringGrid9->Canvas->Brush->Color=clRed;
StringGrid9->Canvas->FillRect(Rect);
StringGrid9->Canvas->TextOutW(Rect.Left+2,Rect.Top+2,StringGrid9->Cells[ACol][ARow]);
}
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid11SelectCell(TObject *Sender, int ACol,
	  int ARow, bool &CanSelect)
{
/*AnsiString  s1,s2;
int k=1;
s1=StringGrid11->Cells[ACol][0];
if(Stolb1<2)
	{
	StringGrid12->ColCount= Stolb1;
	StringGrid12->Cells[Stolb1-1][0]=s1;
	for(int i=0;i<Kolstrok+1;i++)
	StringGrid12->Cells[Stolb1-1][i]=StringGrid2->Cells[ACol+1][i];
	Stolb1++;
	}
else
	{
	for(j=0;j<Stolb1-1;j++)
		{
		s2= StringGrid12->Cells[j+1][0];
		if(s1==s2) k=k*0;
		}
	if(k==1)
		{
		StringGrid12->ColCount= Stolb1;
		StringGrid12->Cells[Stolb1-1][0]=StringGrid11->Cells[ACol][0];
		for(int i=0;i<Kolstrok+1;i++)
		{
		StringGrid12->Cells[Stolb1-1][i]=StringGrid2->Cells[ACol+1][i];
		}
		Stolb1++;
		}
	}*/

AnsiString  s1,s2,s3,s4;
int k=1, t=1;
s1=StringGrid11->Cells[ACol][0];
s3=StringGrid11->Cells[ACol][0]; //Число находящее в ячейке
if(Stolb1<2)
	{
	StringGrid12->ColCount= Stolb1;
	StringGrid12->Cells[Stolb1-1][0]=s1;
	for(int i=0;i<Kolstrok+1;i++)
	StringGrid12->Cells[Stolb1-1][i]=StringGrid2->Cells[ACol+1][i];
	Stolb1++;
	}
else
	{
	for(j=0;j<Stolb1-1;j++)
		{
		s2= StringGrid12->Cells[j+1][0];
		if(s1==s2) k=k*0;
		}
	if(k==1)
		{
		StringGrid12->ColCount= Stolb1;
		StringGrid12->Cells[Stolb1-1][0]=StringGrid11->Cells[ACol][0];
		for(int i=0;i<Kolstrok+1;i++)
		{
		StringGrid12->Cells[Stolb1-1][i]=StringGrid2->Cells[ACol+1][i];
		}
		Stolb1++;
		}
	}

if(Stolb<2)
	{StringGrid8->ColCount= Stolb+1;
	for(int i=0; i<Kolstrok+1; i++)
		{
		StringGrid8->Cells[Stolb][i]=StringGrid2->Cells[ACol+1][i];
		}
	Stolb++;
	}
else
{
for(j=0;j<Stolb-1;j++)
	{
	s4= StringGrid8->Cells[j+1][0];
	if(s3==s4) t=t*0;
	}
if(t==1)
	{ StringGrid8->ColCount= Stolb+1;
	//StringGrid8->Cells[Stolb1-1][0]=StringGrid11->Cells[ACol][0];
	for(int i=0;i<Kolstrok+1;i++)
	{
	StringGrid8->Cells[Stolb][i]=StringGrid2->Cells[ACol+1][i];
	}
	Stolb++;
	}
}

/*
int t=1;
s3=StringGrid11->Cells[ACol][0]; //Число находящее в ячейке
if(Stolb<2)
	{StringGrid8->ColCount= Stolb+1;
	for(int i=0; i<Kolstrok+1; i++)
		{
		StringGrid8->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
		}
	Stolb++;
	}
else
{
for(j=0;j<Stolb-1;j++)
	{
	s4= StringGrid8->Cells[j+1][0];
	if(s1==s2) t=t*0;
	}
if(t==1)
	{ StringGrid8->ColCount= Stolb+1;
	for(int i=0;i<Kolstrok+1;i++)
	{
	StringGrid8->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
	}
	Stolb++;
	}
} */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid7SelectCell(TObject *Sender, int ACol,
	  int ARow, bool &CanSelect)
{
AnsiString s1,s2;
int k=1;
s1=StringGrid7->Cells[ACol][0]; //Число находящее в ячейке
if(Stolb<2)
	{StringGrid2->ColCount= Stolb+1;
	for(int i=0; i<Kolstrok+1; i++)
		{
		StringGrid2->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
		}
	Stolb++;
	}
else
{
for(j=0;j<Stolb-1;j++)
	{
	s2= StringGrid2->Cells[j+1][0];
	if(s1==s2) k=k*0;
	}
if(k==1)
	{ StringGrid2->ColCount= Stolb+1;
	for(int i=0;i<Kolstrok+1;i++)
	{
	StringGrid2->Cells[Stolb][i]=StringGrid3->Cells[ACol+1][i];
	}
	Stolb++;
	}
}
/*
AnsiString  s1,s2;
int k=1;
s1=StringGrid7->Cells[ACol][0];
if(Stolb<2)
	{
	StringGrid11->ColCount= Stolb;
	StringGrid11->Cells[Stolb-1][0]=s1;
	for(int i=0;i<Kolstrok+1;i++)
	StringGrid11->Cells[Stolb-1][i]=StringGrid2->Cells[ACol+1][i];
	Stolb++;
	}
else
	{
	for(j=0;j<Stolb-1;j++)
		{
		s2= StringGrid11->Cells[j+1][0];
		if(s1==s2) k=k*0;
		}
	if(k==1)
		{
		StringGrid11->ColCount= Stolb;
		StringGrid11->Cells[Stolb-1][0]=StringGrid7->Cells[ACol][0];
		for(int i=0;i<Kolstrok+1;i++)
		{
		StringGrid11->Cells[Stolb-1][i]=StringGrid2->Cells[ACol+1][i];
		}
		Stolb++;
		}
	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
for(j=0;j<Stolb1-1;j++)
for(i=0;i<=Kolstrok;i++)
	{
	StringGrid12->Cells[j][i]=" ";
	Mdek[j][i]='\0';
	}
StringGrid12->ColCount=1;
Stolb1=1;
StringGrid8->ColCount=1;
Stolb=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
/*int ji=1,s=0;
for(j=0;j<Stolb1-1;j++) //Stolb1 равен количеству выбранных марок
	for(i=0;i<=Kolstrok;i++) //Количество строк
	{
	Mdek[j][i]=StrToFloat(StringGrid12->Cells[j][i]); //Массив принимает значения из таблицы базы данных
	}
for(j=0;j<Stolb1-2;j++) //Stolb1 равен количеству выбранных марок - 1
	for(i=j+1;i<Stolb1-1;i++) //Stolb1 равен количеству выбранных марок
	{
		for(s=0;s<=Kolstrok;s++) //Количество строк ([ходит по столбцам)
		{
		if(s>0)
			{
			StringGrid8->Cells[ji][s]=FloatToStr(sokr(fabs(Mdek[j][s]-Mdek[i][s]), 10000));
			//StringGrid9->Cells[ji][s]=fabs(sokr((FloatToStr(Mdek[j][s]-Mdek[i][s])-FloatToStr(Mdek[j][1]-Mdek[i][1])),10000));
			}
		else
			{
			StringGrid8->Cells[ji][s]=FloatToStr(Mdek[j][s])+"-"+FloatToStr(Mdek[i][s]);
			//StringGrid9->Cells[ji][s]=FloatToStr(Mdek[j][s])+"-"+FloatToStr(Mdek[i][s]);
			}
		StringGrid8->ColCount=ji+1;
		//StringGrid9->ColCount=ji+1;
		}
	ji++;
   }*/

for(j=0;j<Kolstrolb;j++)
	for(i=0;i<Kolstrok;i++)
	IshodDan[j][i]=0;
for(j=0;j<Stolb1-1;j++)
	for(i=0;i<Kolstrok;i++)
	IshodDan[j][i]=StrToFloat(StringGrid12->Cells[j][i+1]);
	raschet_MUGL_i_AlphaGL();
	raschet_MU_prog();
	raschet_ustoi();
	for(i=0;i<Kolstrok;i++)
       {
	   StringGrid13->Cells[1][i+1]=FloatToStr(MUGL[i]);
	   StringGrid13->Cells[2][i+1] = FloatToStrF(AlphaGL[i]*10.0,ffFixed,9,9);
	   }

for(i=0;i<=Kolstrok;i++)
StringGrid13->Cells[3][i+1]=FloatToStr(MUprog[i]);
for(i=0;i<=Kolstrok;i++)
{
StringGrid13->Cells[4][i+1] = "0,0" + FloatToStr(ALprog[i]*10.0).SubString(3,9);
}
StringGrid13->Cells[0][Kolstrok+1]="Прог" ;

for( i=0;i<Kolstrok+1;i++)
if(MUpm[i]>=MUi[i])
StringGrid13->Cells[5][i+1]="+";
else StringGrid13->Cells[5][i+1]="-";

double x,y,a,b,a1,b1,x1,y1;
Series16->Clear();
Series17->Clear();
Series18->Clear();
Series19->Clear();
for(int i=0; i<Kolstrok-1; i++)
	{
	x=MUGL[i];
	y=AlphaGL[i];
	a=MUprog[i];
	b=ALprog[i];
	Series16->AddXY(x,y);
	Series18->AddXY(x,y);
	Series19->AddXY(a,b);
	Series17->AddXY(a,b);
	}
Chart5->Refresh();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TreeView1Click(TObject *Sender)
{
if (TreeView1->Selected->Text=="Инструкция")
{
PageControl1->ActivePageIndex=0;
}
if (TreeView1->Selected->Text=="Загрузка данных")
{
PageControl1->ActivePageIndex=1;
}
if (TreeView1->Selected->Text=="Система")
{
PageControl1->ActivePageIndex=2;
}
if (TreeView1->Selected->Text=="Блок")
{
PageControl1->ActivePageIndex=3;
}
if (TreeView1->Selected->Text=="Подблок")
{
PageControl1->ActivePageIndex=4;
}
if (TreeView1->Selected->Text=="Марка")
{
PageControl1->ActivePageIndex=5;
}
if (TreeView1->Selected->Text=="Методические материалы")
{
PageControl1->ActivePageIndex=6;
}
if (TreeView1->Selected->Text=="Настройки")
{
Form4->Show();
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
Panel1->Width=Form1->ClientWidth*0.19-Panel1->Left-2;

TreeView1->Top=Form1->ClientHeight*0.09+2;
TreeView1->Height=ClientHeight*0.4+2;
TreeView1->Font->Height=ClientHeight*0.04;
TreeView1->Width=ClientWidth*0.19+2;

Image1->Left=2;
Image1->Width=ClientWidth*0.18+2;
Image1->Top=Form1->ClientHeight*0.67-2;
Image1->Height=ClientHeight*0.28+2;



RichEdit1->Left=Form1->ClientWidth*0.19+2;
RichEdit1->Width=Form1->ClientWidth*0.4-2;
RichEdit1->Top=Form1->ClientHeight*0.05+2;
RichEdit1->Height=ClientHeight*0.83-2;
RichEdit1->Font->Height=ClientHeight*0.028;

Label9->Left=Form1->ClientWidth*0.3+2;
Label9->Width=Form1->ClientWidth*0.3-2;
Label9->Top=Form1->ClientHeight*0.01+2;
Label9->Height=ClientHeight*0.05-2;
Label9->Font->Height=ClientHeight*0.04;

Panel2->Left=Form1->ClientWidth*0.6+2;
Panel2->Width=Form1->ClientWidth*0.39-2;
Panel2->Top=Form1->ClientHeight*0.05+2;
Panel2->Height=ClientHeight*0.4-2;

Panel3->Left=Form1->ClientWidth*0.6+2;
Panel3->Width=Form1->ClientWidth*0.39-2;
Panel3->Top=Form1->ClientHeight*0.49+2;
Panel3->Height=ClientHeight*0.4-2;

Label21->Left=Form1->ClientWidth*0.62+2;
Label21->Width=Form1->ClientWidth*0.3-2;
Label21->Top=Form1->ClientHeight*0.01+2;
Label21->Height=ClientHeight*0.05-2;
Label21->Font->Height=ClientHeight*0.04;

Label22->Left=Form1->ClientWidth*0.62+2;
Label22->Width=Form1->ClientWidth*0.3-2;
Label22->Top=Form1->ClientHeight*0.45+2;
Label22->Height=ClientHeight*0.05-2;
Label22->Font->Height=ClientHeight*0.04;



StringGrid3->Left=Form1->ClientWidth*0.19+2;
StringGrid3->Width=Form1->ClientWidth*0.92-StringGrid3->Left-2;
StringGrid3->Top=Form1->ClientHeight*0.14+2;
StringGrid3->Height=ClientHeight*0.7-2;
StringGrid3->Font->Height=ClientHeight*0.03;
StringGrid3->DefaultColWidth=ClientHeight*0.1;
StringGrid3->DefaultRowHeight=ClientHeight*0.025;

Button1->Left=Form1->ClientWidth*0.19+2;
Button1->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button1->Top=Form1->ClientHeight*0.01+2;
Button1->Height=ClientHeight*0.07-2;
Button1->Font->Height=ClientHeight*0.025;

Button2->Left=Form1->ClientWidth*0.41+2;
Button2->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button2->Top=Form1->ClientHeight*0.01+2;
Button2->Height=ClientHeight*0.07-2;
Button2->Font->Height=ClientHeight*0.025;

Button4->Left=Form1->ClientWidth*0.825+2;
Button4->Width=Form1->ClientWidth*0.35-StringGrid3->Left-2;
Button4->Top=Form1->ClientHeight*0.01+2;
Button4->Height=ClientHeight*0.07-2;
Button4->Font->Height=ClientHeight*0.025;

ComboBox1->Left=Form1->ClientWidth*0.3+2;
ComboBox1->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
ComboBox1->Top=Form1->ClientHeight*0.05+2;
ComboBox1->Height=ClientHeight*0.04-2;
ComboBox1->Font->Height=ClientHeight*0.02;

Label8->Left=Form1->ClientWidth*0.3+2;
Label8->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Label8->Top=Form1->ClientHeight*0.01+2;
Label8->Height=ClientHeight*0.04-2;
Label8->Font->Height=ClientHeight*0.03;

Label5->Left=Form1->ClientWidth*0.45+2;
Label5->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Label5->Top=Form1->ClientHeight*0.095+2;
Label5->Height=ClientHeight*0.04-2;
Label5->Font->Height=ClientHeight*0.05;



StringGrid1->Left=Form1->ClientWidth*0.19+2;
StringGrid1->Width=Form1->ClientWidth*0.395-StringGrid1->Left-2;
StringGrid1->Top=Form1->ClientHeight*0.09+2;
StringGrid1->Height=ClientHeight*0.45-2;

StringGrid4->Left=Form1->ClientWidth*0.395+2;
StringGrid4->Width=Form1->ClientWidth*0.38-StringGrid1->Left-2;
StringGrid4->Top=Form1->ClientHeight*0.09+2;
StringGrid4->Height=ClientHeight*0.45-2;

StringGrid5->Left=Form1->ClientWidth*0.585+2;
StringGrid5->Width=Form1->ClientWidth*0.6-StringGrid1->Left-2;
StringGrid5->Top=Form1->ClientHeight*0.09+2;
StringGrid5->Height=ClientHeight*0.45-2;

Chart1->Left=Form1->ClientWidth*0.19+2;
Chart1->Width=Form1->ClientWidth*0.57-StringGrid1->Left-2;
Chart1->Top=Form1->ClientHeight*0.545+2;
Chart1->Height=ClientHeight*0.42-2;

Chart2->Left=Form1->ClientWidth*0.57+2;
Chart2->Width=Form1->ClientWidth*0.615-StringGrid1->Left-2;
Chart2->Top=Form1->ClientHeight*0.545+2;
Chart2->Height=ClientHeight*0.42-2;

Label6->Left=Form1->ClientWidth*0.22+2;
Label6->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label6->Top=Form1->ClientHeight*0.01+2;
Label6->Height=ClientHeight*0.3-2;
Label6->Font->Height=ClientHeight*0.03;

Label7->Left=Form1->ClientWidth*0.54+2;
Label7->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label7->Top=Form1->ClientHeight*0.01+2;
Label7->Height=ClientHeight*0.3-2;
Label7->Font->Height=ClientHeight*0.03;

Label11->Left=Form1->ClientWidth*0.23+2;
Label11->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label11->Top=Form1->ClientHeight*0.055+2;
Label11->Height=ClientHeight*0.3-2;
Label11->Font->Height=ClientHeight*0.034;

Label12->Left=Form1->ClientWidth*0.40+2;
Label12->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label12->Top=Form1->ClientHeight*0.055+2;
Label12->Height=ClientHeight*0.3-2;
Label12->Font->Height=ClientHeight*0.034;

Label13->Left=Form1->ClientWidth*0.70+2;
Label13->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label13->Top=Form1->ClientHeight*0.055+2;
Label13->Height=ClientHeight*0.3-2;
Label13->Font->Height=ClientHeight*0.034;

Edit2->Left=Form1->ClientWidth*0.37+2;
Edit2->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Edit2->Top=Form1->ClientHeight*0.01+2;
Edit2->Height=ClientHeight*0.3-2;
Edit2->Font->Height=ClientHeight*0.02;

Edit3->Left=Form1->ClientWidth*0.68+2;
Edit3->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Edit3->Top=Form1->ClientHeight*0.01+2;
Edit3->Height=ClientHeight*0.3-2;
Edit3->Font->Height=ClientHeight*0.02;

Button7->Left=Form1->ClientWidth*0.885+2;
Button7->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button7->Top=Form1->ClientHeight*0.01+2;
Button7->Height=ClientHeight*0.06-2;
Button7->Font->Height=ClientHeight*0.025;



StringGrid2->Left=Form1->ClientWidth*0.19+2;
StringGrid2->Width=Form1->ClientWidth*0.592-StringGrid2->Left-2;
StringGrid2->Top=Form1->ClientHeight*0.09+2;
StringGrid2->Height=ClientHeight*0.40-2;

StringGrid6->Left=Form1->ClientWidth*0.592+2;
StringGrid6->Width=Form1->ClientWidth*0.592-StringGrid2->Left-2;
StringGrid6->Top=Form1->ClientHeight*0.09+2;
StringGrid6->Height=ClientHeight*0.40-2;

StringGrid7->Left=Form1->ClientWidth*0.32+2;
StringGrid7->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
StringGrid7->Top=Form1->ClientHeight*0.011+2;
StringGrid7->Height=ClientHeight*0.045-2;
StringGrid7->Font->Height=ClientHeight*0.025;
StringGrid7->DefaultColWidth=ClientHeight*0.035;
StringGrid7->DefaultRowHeight=ClientHeight*0.045;

Chart3->Left=Form1->ClientWidth*0.30+2;
Chart3->Width=Form1->ClientWidth*0.8-StringGrid2->Left-2;
Chart3->Top=Form1->ClientHeight*0.495+2;
Chart3->Height=ClientHeight*0.47-2;

Label1->Left=Form1->ClientWidth*0.20+2;
Label1->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label1->Top=Form1->ClientHeight*0.011+2;
Label1->Height=ClientHeight*0.3-2;
Label1->Font->Height=ClientHeight*0.04;

Label14->Left=Form1->ClientWidth*0.33+2;
Label14->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label14->Top=Form1->ClientHeight*0.055+2;
Label14->Height=ClientHeight*0.3-2;
Label14->Font->Height=ClientHeight*0.04;

Label15->Left=Form1->ClientWidth*0.7+2;
Label15->Width=Form1->ClientWidth*0.3-StringGrid1->Left-2;
Label15->Top=Form1->ClientHeight*0.055+2;
Label15->Height=ClientHeight*0.3-2;
Label15->Font->Height=ClientHeight*0.04;

Button10->Left=Form1->ClientWidth*0.585+2;
Button10->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button10->Top=Form1->ClientHeight*0.01+2;
Button10->Height=ClientHeight*0.06-2;
Button10->Font->Height=ClientHeight*0.025;

Button9->Left=Form1->ClientWidth*0.885+2;
Button9->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button9->Top=Form1->ClientHeight*0.01+2;
Button9->Height=ClientHeight*0.06-2;
Button9->Font->Height=ClientHeight*0.025;



StringGrid9->Left=Form1->ClientWidth*0.19+2;
StringGrid9->Width=Form1->ClientWidth*0.4175-StringGrid9->Left-2;
StringGrid9->Top=Form1->ClientHeight*0.14+2;
StringGrid9->Height=ClientHeight*0.40-2;

StringGrid8->Left=Form1->ClientWidth*0.4177+2;
StringGrid8->Width=Form1->ClientWidth*0.4175-StringGrid9->Left-2;
StringGrid8->Top=Form1->ClientHeight*0.14+2;
StringGrid8->Height=ClientHeight*0.40-2;

StringGrid13->Left=Form1->ClientWidth*0.6457+2;
StringGrid13->Width=Form1->ClientWidth*0.54-StringGrid9->Left-2;
StringGrid13->Top=Form1->ClientHeight*0.14+2;
StringGrid13->Height=ClientHeight*0.40-2;

StringGrid11->Left=Form1->ClientWidth*0.19+2;
StringGrid11->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
StringGrid11->Top=Form1->ClientHeight*0.036+2;
StringGrid11->Height=ClientHeight*0.045-2;
StringGrid11->Font->Height=ClientHeight*0.025;
StringGrid11->DefaultColWidth=ClientHeight*0.035;
StringGrid11->DefaultRowHeight=ClientHeight*0.045;

StringGrid12->Left=Form1->ClientWidth*0.43+2;
StringGrid12->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
StringGrid12->Top=Form1->ClientHeight*0.036+2;
StringGrid12->Height=ClientHeight*0.045-2;
StringGrid12->Font->Height=ClientHeight*0.025;
StringGrid12->DefaultColWidth=ClientHeight*0.035;
StringGrid12->DefaultRowHeight=ClientHeight*0.045;

Chart5->Left=Form1->ClientWidth*0.30+2;
Chart5->Width=Form1->ClientWidth*0.8-StringGrid2->Left-2;
Chart5->Top=Form1->ClientHeight*0.545+2;
Chart5->Height=ClientHeight*0.42-2;

Label2->Left=Form1->ClientWidth*0.27+2;
Label2->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label2->Top=Form1->ClientHeight*0.001+2;
Label2->Height=ClientHeight*0.045-2;
Label2->Font->Height=ClientHeight*0.04;

Label3->Left=Form1->ClientWidth*0.51+2;
Label3->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label3->Top=Form1->ClientHeight*0.001+2;
Label3->Height=ClientHeight*0.045-2;
Label3->Font->Height=ClientHeight*0.04;

Label16->Left=Form1->ClientWidth*0.27+2;
Label16->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label16->Top=Form1->ClientHeight*0.1+2;
Label16->Height=ClientHeight*0.045-2;
Label16->Font->Height=ClientHeight*0.04;

Label17->Left=Form1->ClientWidth*0.49+2;
Label17->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label17->Top=Form1->ClientHeight*0.1+2;
Label17->Height=ClientHeight*0.045-2;
Label17->Font->Height=ClientHeight*0.04;

Label18->Left=Form1->ClientWidth*0.71+2;
Label18->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label18->Top=Form1->ClientHeight*0.1+2;
Label18->Height=ClientHeight*0.045-2;
Label18->Font->Height=ClientHeight*0.04;

Button5->Left=Form1->ClientWidth*0.685+2;
Button5->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button5->Top=Form1->ClientHeight*0.01+2;
Button5->Height=ClientHeight*0.06-2;
Button5->Font->Height=ClientHeight*0.025;

Button6->Left=Form1->ClientWidth*0.885+2;
Button6->Width=Form1->ClientWidth*0.3-StringGrid3->Left-2;
Button6->Top=Form1->ClientHeight*0.01+2;
Button6->Height=ClientHeight*0.06-2;
Button6->Font->Height=ClientHeight*0.025;



Chart6->Left=Form1->ClientWidth*0.25+2;
Chart6->Width=Form1->ClientWidth*0.87-StringGrid2->Left-2;
Chart6->Top=Form1->ClientHeight*0.05+2;
Chart6->Height=ClientHeight*0.9-2;

CheckListBox1->Font->Height=ClientHeight*0.025;


Label19->Left=Form1->ClientWidth*0.85+2;
Label19->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label19->Top=Form1->ClientHeight*0.01+2;
Label19->Height=ClientHeight*0.045-2;
Label19->Font->Height=ClientHeight*0.04;

Label20->Left=Form1->ClientWidth*0.36+2;
Label20->Width=Form1->ClientWidth*0.425-StringGrid3->Left-2;
Label20->Top=Form1->ClientHeight*0.01+2;
Label20->Height=ClientHeight*0.045-2;
Label20->Font->Height=ClientHeight*0.04;

RichEdit2->Left=Form1->ClientWidth*0.745+2;
RichEdit2->Width=Form1->ClientWidth*0.25-2;
RichEdit2->Top=Form1->ClientHeight*0.05+2;
RichEdit2->Height=ClientHeight*0.9-2;
RichEdit2->Font->Height=ClientHeight*0.02;

CppWebBrowser2->Left=Form1->ClientWidth*0.19+2;
CppWebBrowser2->Width=Form1->ClientWidth*0.55-2;
CppWebBrowser2->Top=Form1->ClientHeight*0.05+2;
CppWebBrowser2->Height=ClientHeight*0.4-2;

CppWebBrowser1->Left=Form1->ClientWidth*0.19+2;
CppWebBrowser1->Width=Form1->ClientWidth*0.55-2;
CppWebBrowser1->Top=Form1->ClientHeight*0.45+2;
CppWebBrowser1->Height=ClientHeight*0.53-2;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Image1Click(TObject *Sender)
{
if (ComboBox1->Text!="") //Если текст в ComboBox1 присутствует
{
Form2->Show();
Form2->Image1->Picture->LoadFromFile(""+OpenDialog1->FileName+".bmp"); //Загрузка изображения варианта
}
}
//---------------------------------------------------------------------------

int GetMarksColumn(String MarkName)
{
int col=-1;
 for(int i=1;i<Form1->StringGrid3->ColCount;i++)
	if (Form1->StringGrid3->Cells[i][0]==MarkName) {
	 col=i;
	}
return col;
}



void __fastcall TForm1::CheckListBox1ClickCheck(TObject *Sender)
{
int days, col;
double y;
String xlabel;
days=StringGrid3->RowCount-1; //Кол-во строк в исходной таблице - 1
Chart6->SeriesList->Clear(); //Очищение графика Chart 6
TGDIPlusCanvas *myGDIPlus = new TGDIPlusCanvas();
Chart6->Canvas = myGDIPlus;
Chart6->LeftAxis->Increment=0.001;
Chart6->Refresh();
for (int k=0; k<CheckListBox1->Count;k++) //Перебор значения марок
  {
   if (CheckListBox1->Checked[k]==true) //Выбор каждой последующей марки
	{
	  col=GetMarksColumn(CheckListBox1->Items->Strings[k]);
	  if (col==-1) return;
	  TLineSeries *Ser = new TLineSeries(NULL); //Создание новой Series
	  Ser->ParentChart=Chart6; //Присваивание Ser к Chart 6

	  Ser->Title=CheckListBox1->Items->Strings[k]; //
	  Ser->Name="Mark"+CheckListBox1->Items->Strings[k];
	  Ser->Legend->Text="Марка "+CheckListBox1->Items->Strings[k];
	  Ser->Pen->Width=2; //Ширина пера
	  Ser->Pointer->Size=3; //Размер точек
	  Ser->Pointer->SizeUnits=suPixels;
	  Ser->Pointer->Style=psCircle; //Стиль точек
	  Ser->Pointer->Visible=true;
	  Ser->Pointer->Pen->Visible=false;
	  Ser->Marks->Visible=true;
	  Ser->Marks->Style=smsValue;
	  Ser->Marks->Transparent=true;
	  Ser->Marks->FontSeriesColor=true;

	  for(int d=1;d<=days;d++)  //Перебор значений всех дней
		{
		xlabel=StringGrid3->Cells[0][d];
		y=StrToFloat(StringGrid3->Cells[col][d]);  //Сбор значений из таблицы исходников
		Ser->AddXY(d,y,xlabel);
		}
	}
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
CppWebBrowser1->Show();
CppWebBrowser2->Show();

CppWebBrowser1->Stop();
CppWebBrowser2->Stop();
CppWebBrowser1->Quit();
CppWebBrowser2->Quit();
MediaPlayer1->Stop();
MediaPlayer2->Stop();
MediaPlayer1->Close();
MediaPlayer2->Close();
Form4->INI->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
/*RichEdit2->HideSelection = false;
//RichEdit2->SelStart=RichEdit2->Text.Pos(Edit1->Text)-1;
RichEdit2->SelStart=i-RichEdit2->Perform(EM_EXLINEFROMCHAR, 0, RichEdit1->SelStart)-1;
RichEdit2->SelLength=Edit1->Text.Length();       */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel2MouseEnter(TObject *Sender)
{
MediaPlayer1->Refresh();
MediaPlayer1->Play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel2MouseLeave(TObject *Sender)
{
MediaPlayer1->Pause();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel3MouseEnter(TObject *Sender)
{
MediaPlayer2->Refresh();
MediaPlayer2->Play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel3MouseLeave(TObject *Sender)
{
MediaPlayer2->Stop();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
PageControl1->ActivePageIndex=0;
TreeView1->Select(TreeView1->Items->Item[0]);
}
//---------------------------------------------------------------------------

