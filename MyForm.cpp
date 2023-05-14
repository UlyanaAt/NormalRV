#define _USE_MATH_DEFINES
#include "MyForm.h"
#include <iostream>
#include <time.h>
#include <math.h>

using namespace NormalRV;
using namespace System::Windows::Forms;

// Application entry point
[STAThreadAttribute]
void main(array<String^>^ args) {
	srand(time(NULL));
	// Initial Application Parameters:
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	NormalRV::MyForm form;
	Application::Run(% form);
}

System::Void NormalRV::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	double mean = 0, variance = 0, statMean = 0, statVar = 0, min = 0, max = 0, pi = 0, px = 0, x = 0, Chi = 0;
	int size = 0;

	mean = (double)numericUpDown3->Value;
	variance = (double)numericUpDown2->Value;
	size = (int)numericUpDown1->Value;

	double* normalRVs = new double[size];
	double* ChiValue = new double[12] { 3.841, 5.991, 7.815, 9.488, 11.070, 12.592, 14.067, 15.507, 16.919, 18.307, 19.675, 21.026 };

	chart1->Series["Series1"]->Points->Clear();

	for (int i = 0; i < size; i++)
	{
		//creating a normal random variables
		double x = 0, y = 0;
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;

		double r = sqrt(-2 * log(x));
		double theta = 2 * M_PI * y;

		double z1 = r * cos(theta);

		normalRVs[i] = z1;

		//find min and max of normalRVs
		if (i == 0)
		{
			min = normalRVs[i];
			max = normalRVs[i];
		}
		else
		{
			if (normalRVs[i] < min)
			{
				min = normalRVs[i];
			}
			if (normalRVs[i] > max)
			{
				max = normalRVs[i];
			}
		}
		//find statistic mean and variance of normalRVs
		statMean += normalRVs[i];
		statVar += normalRVs[i]* normalRVs[i];
	}
	statMean = statMean / size;
	statVar = statVar / size - statMean * statMean;

	//find number of columns
	int numCol = 0;
	numCol = (int)log2(size) + 1;
	double step = (max - min) / numCol;

	int* colCount = new int[numCol];
	for (int i = 0; i < numCol; i++)
	{
		colCount[i] = 0;
	}

	//Distribute array normalRVs's values over array colCount in increments of step
	double curLength;
	for (int i = 0; i < size; i++)
	{
		curLength = min + step;
		int j = 0;
		while (j < numCol)
		{
			if (normalRVs[i] < curLength)
			{ 
				colCount[j]++;
				break;
			}
			else
			{
				curLength += step;
				j++;
			}
			
		}
	}

	double* columnFreq = new double[numCol];

	for (int i = 0; i < numCol; i++)
	{
		columnFreq[i] = 0;
	}
	double min1 = min;

	for (int i = 0; i < numCol; i++)
	{
		columnFreq[i] = (double)colCount[i] / size;
		x = (min1 + min1 + step) / 2;
		px = (1 / (Math::Sqrt(2 * Math::PI) * Math::Sqrt(variance))) * Math::Exp(-((x - mean) * (x - mean)) / (2 * variance));
		pi = ((min1 + step) - min1) * px;
		Chi += (colCount[i] * colCount[i]) / (double)(size * pi);
		chart1->Series["Series1"]->Points->AddXY(min1, columnFreq[i]);
		min1 += step;
	}
	Chi -= size;

	label7->Text = statMean.ToString();
	label8->Text = statVar.ToString();

	if (Chi > ChiValue[numCol - 2])
	{
		label9->Text = Chi.ToString() + " > " + ChiValue[numCol - 2] + " is true";
	}
	else
	{
		label9->Text = Chi.ToString() + " > " + ChiValue[numCol - 2] + " is false";
	}

}
