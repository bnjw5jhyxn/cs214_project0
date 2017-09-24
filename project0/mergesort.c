#include <stdlib.h>
#include "sorter.h"
int sort_by_field(struct csv *table, const char *field_name)
{
	int field_index;
	int *end;
	int low, middle, high;
	int i, j;
	struct field **a, **b;
	int index;
	int number_of_headers, number_of_records;
	char **header_strings;
	struct field **values;
	/* performs a bottom-up mergesort on the table */
	number_of_headers = table->number_of_headers;
	number_of_records = table->number_of_records;
	header_strings = table->header_strings;
	values = table->values;
	/* find the column number for comparisons */
	field_index = -1;
	for (i = 0; i < number_of_headers; i++) {
		if (strcmp(header_strings[i], field_name) == 0) {
			field_index = i;
			break;
		}
	}
	if (field_index == -1) {
		return 1;
	}
	/* find already sorted regions */
	end = malloc(number_of_records * sizeof(int));
	j = 0;
	for (i = 0; i < number_of_records; i++) {
		if (compare(values[i - 1] + field_index, values[i] + field_index) > 0) {
			end[j] = i;
			j = i;
		}
	}
	end[j] = number_of_records;
	/* begin actual mergesort */
	a = values;
	b = malloc(number_of_records * sizeof(struct field *));
	index = 0;
	while (end[0] != number_of_records) {
		low = index;
		middle = end[index];
		high = end[middle];
		i = low;
		j = middle;
		while (i < middle && j < high) {
			if (compare(a[i] + field_index, a[j] + field_index) <= 0)
				b[index++] = a[i++];
			else
				b[index++] = a[j++];
		}
		while (i < middle)
			b[index++] = a[i++];
		while (j < high)
			b[index++] = a[j++];
		end[low] = high;
		if (high == n || end[high] == n) {
			/* reset index to 0, then swap a and b */
			for (; index < n; index++)
				b[index] = a[index];
			index = 0;
			a ^= b;
			b ^= a;
			a ^= b;
		}
	}
	if (values != a) {
		table->values = a;
	}
	free(end);
	free(b);
}

int compare(struct field *a, struct field *b) {
	if (a->string == NULL && b->string == NULL) {
		if (a->number < b->number)
			return -1;
		else if (a->number > b->number)
			return 1;
		else
			return 0
	} else if (a->string == NULL) {
		if (strlen(b->string) == 0)
			return 1;
		else
			return -1;
	} else if (b->string == NULL) {
		if (strlen(a->string) == 0)
			return -1;
		else
			return 1;
	} else {
		return strcmp(a->string, b->string);
	}
}
