// crypto_assignment_3.cpp : Defines the entry point for the console application.
//


//sigur merge! de aia nu am testat nimic
//unsigned char si char mmmm

#define HMAC_LENGTH 20

#include "stdafx.h"
#include <string.h>
#include <openssl\hmac.h>
#include <intrin.h>
#include <math.h>

char* Function(char* password, char* salt, int iterations_count, int i)
{
	char* result = new char[HMAC_LENGTH];
	//hello U
	unsigned char** U = new unsigned char*[iterations_count];
	//initialising start
	char* start = new char[strlen(salt) + 8];
	memset(start, 0, strlen(salt) + 8);
	snprintf(start, strlen(salt) + 8, "%s%x", salt, _byteswap_uint64(i));
	//U stuff
	U[0] = HMAC(EVP_sha1(), password, strlen(password), (unsigned char*)start, strlen(start), NULL, NULL);
	strcpy_s(result, HMAC_LENGTH, (char*)U[0]);
	for (int j = 1; j < iterations_count; j++)
	{
		U[j] = HMAC(EVP_sha1(), password, strlen(password), U[j - 1], HMAC_LENGTH, NULL, NULL);
		//the grand XOR
		for (int i = 0; i < HMAC_LENGTH; i++)
		{
			result[i] = result[i] ^ U[j][i];
		}
	}
	return result;
}

char* Pbkdf2(char* password, char* salt, int iterations_count, int desired_dkey_length)
{
	char* d_key = new char[desired_dkey_length];
	int l = ceil(desired_dkey_length / HMAC_LENGTH);
	int r = desired_dkey_length - (l - 1) * HMAC_LENGTH;

	int offset = 0;
	//??who is R?
	for (int i = 1; i <= l; i++)
	{//maybe use i+1 where needed and not ruin tens of years of for from 0
		char* iteration = Function(password, salt, iterations_count, i);
		//misto notatie || inseamna concatenare obvs
		memcpy_s(d_key + offset, HMAC_LENGTH, iteration, HMAC_LENGTH);
		offset += HMAC_LENGTH;
	}
	return d_key;
}


int main()
{
	return 0;
}

