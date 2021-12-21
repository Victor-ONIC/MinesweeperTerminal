#pragma once

#include <iostream>
#include <vector>

#include "func.h"

void menu(int &row, int &col, int &d);

void display(int row, int col);
void display(Matrix *matrix);

void display_banner();

void act(Matrix *matrix, coord_pair &p);
void act(Matrix *matrix, int &a, coord_pair &p);

void func_clear(void);
