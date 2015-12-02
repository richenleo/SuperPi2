// SupertPi2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
//#define L 20000 
//#define N L/4+1

int L, N;

void add(int*, int*, int*);
void sub(int*, int*, int*);
void div(int*, int, int*);
void calc(void);

int main(int argc, char** argv)
{
	SYSTEMTIME lt;

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	L = atoi(argv[1]);
	N = (int)(L / 4) + 1;
	int iSleepSec = atoi(argv[2]);

	while (true)
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);

		calc();

		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

		ElapsedMicroseconds.QuadPart *= 1000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		GetLocalTime(&lt);
		printf("%02d:%02d:%02d.%03d \t Frequency:%I64d \t Latency:%I64dms\n", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds, Frequency.QuadPart, ElapsedMicroseconds.QuadPart);

		SleepEx(iSleepSec*1000, false);
	}

	return 0;

}

void calc(void)
{
	int *s = new int[N + 3];
	int *w = new int[N + 3];	
	int *v = new int[N + 3];	
	int *q = new int[N + 3];
	memset(s, 0, sizeof(int) * (N + 3));	
	memset(w, 0, sizeof(int) * (N + 3));
	memset(v, 0, sizeof(int) * (N + 3));
	memset(q, 0, sizeof(int) * (N + 3));

	//int s[N + 3] = { 0 };
	//int w[N + 3] = { 0 };
	//int v[N + 3] = { 0 };
	//int q[N + 3] = { 0 };
	int n = (int)(L / 1.39793 + 1);
	int k;

	w[0] = 16 * 5;
	v[0] = 4 * 239;

	for (k = 1; k <= n; k++)
	{
		div(w, 25, w);
		div(v, 239, v);
		div(v, 239, v);
		sub(w, v, q);
		div(q, 2 * k - 1, q);

		if (k % 2)
			add(s, q, s);
		else
			sub(s, q, s);
	}
	
	int j;
	char *result = new char[L + 2];
	memset(result, 0, sizeof(char) * (L + 2));
	char *curresult = result;
	sprintf(result, "%d.", s[0]);
	curresult += 2;
	for (k = 1; k < N; k++)
	{
		j=sprintf(curresult, "%04d", s[k]);
		curresult += 4;
	}

	delete []s;
	delete []w;
	delete []v;
	delete []q;
	delete []result;
	//printf("%s\n",result);

}

void add(int *a, int *b, int *c)
{
	int i, carry = 0;

	for (i = N + 1; i >= 0; i--)
	{
		c[i] = a[i] + b[i] + carry;
		if (c[i] < 10000)
			carry = 0;
		else   
		{
			c[i] = c[i] - 10000;
			carry = 1;
		}
	}
}

void sub(int *a, int *b, int *c)
{
	int i, borrow = 0;
	for (i = N + 1; i >= 0; i--)
	{
		c[i] = a[i] - b[i] - borrow;
		if (c[i] >= 0)
			borrow = 0;
		else   
		{
			c[i] = c[i] + 10000;
			borrow = 1;
		}
	}
}

void div(int *a, int b, int *c)
{
	int i, tmp, remain = 0;
	for (i = 0; i <= N + 1; i++)
	{
		tmp = a[i] + remain;
		c[i] = tmp / b;
		remain = (tmp % b) * 10000;
	}
}
