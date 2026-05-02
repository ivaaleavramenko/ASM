#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/*
Дан массив A из 16 байтов (числа знаковые). Вычислить элементы байтового массива B по формуле B(k) = sum (-1)^i*A(i). Сосчитать количество элементов массива B, при вычислении которых возникает переполнение и поместить адреса (смещения) этих элементов в массив C.
*/

int main()
{
	char			A[16] = { 100, -100, 1, 8, 127, 0, 127, 127, 7, 8, 10, 12, 13, 14, 15, 16 };
	unsigned		size_ = 16;
	unsigned char   i, overflow_count = 0;


	char B[16];
	unsigned int C[16];
	


	printf("Array A:\n");
	//print_arr(size_, A);
	for (i = 0; i < size_; i++)
		printf("A[%d]: %d -> %x\n", i, A[i], &A[i]);



	__asm {

		lea esi, A      ; адрес начала массива А в esi
		lea edi, B      ; адрес начала массива B в edi
		lea edx, C      ; адрес начала массива C в edx

		xor al, al      ; аккумулятор суммы

		mov ecx, size_  ; счётчик цикла
		mov ah, 0       ; знак слагаемого(если 0, то + , иначе - )

		jecxz done      ; если массив пуст, то завершаем программу

	L:
		cmp ah, 0       ; проверка знака
		jz plus         ; если ah = 0, то +
		                ; иначе ah = 1, значит, -
		xor ah, ah      ; обнуляем ah, чтобы следующая операция была +
		sub al, [esi]   ; al -= A[i]
		jo overflow	    ; проверка переполнения	
		jmp next        ; переходим к следующему этапу

	plus:
		inc ah          ; делаем ah = 1, чтобы следующая операция была -
		add al, [esi]   ; al += A[i]
		jo overflow     ; проверка переполнения
		jmp next        ; переходим к следующему этапу

	overflow:
		xor al, al				; при переполнении сбрасываем сумму
		inc overflow_count      ; увеличиваем счётчик переполнения на 1
		mov [edx], esi          ; сохраняем результат в массив C
		add edx, 4              ; сдвигаем адрес эл-та С на 4 байта

	next:
		mov [edi], al   ; сохраняем результат вычислений в массив В
		inc esi         ; переходим к следующему адресу массива А
		inc edi         ; переходим к следующему адресу массива B
		loop L          ; ecx -= 1, если ecx != 0, то продолжаем цикл

	done:
		nop             ; выход из ассемблерной вставки
	}

	printf("\nArray B:\n");
	for (i = 0; i < size_; ++i)
		printf("B[%d]: %d -> %x\n", i, B[i], &B[i]);

	printf("\nQuantity of overflows: %u\n", overflow_count);

	if (overflow_count) {
		printf("\nArray C:\n");
		for (int i = 0; i < overflow_count; ++i)
			printf("C[%d]: %x\n", i, C[i]);
	}

	return 0;
}
