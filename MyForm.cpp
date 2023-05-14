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
	double mean = 0, variance = 0, statMean = 0, statVar = 0, min = 0, max = 0;
	int size = 0;

	double* normalRVs = new double[size];

	mean = (double)numericUpDown3->Value;
	variance = (double)numericUpDown2->Value;
	size = (int)numericUpDown1->Value;

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

	//Distribute array normalRVs's values over array colCount in increments of step
	for (int i = 0; i < size; i++)
	{
		int j = 0;
		while (normalRVs[i] > min + step * j)
		{
			j++;
		}
		colCount[j]++;
	}

	double* columnFreq = new double[numCol];
	double min1 = min;

	for (int i = 0; i < numCol; i++)
	{
		columnFreq[i] = (double)colCount[i] / size;
		chart1->Series[0]->Points->AddXY(min1, columnFreq[i]);
		min1 += step;
	}

}
