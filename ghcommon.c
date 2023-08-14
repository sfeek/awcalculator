#include "ghcommon.h"

int append_string(char **s1, char *s2)
{
	char *t;
	size_t len_s1, len_s2;

	if (s2 == NULL)
		return SUCCESS;

	if ((len_s2 = strlen(s2)) == 0)
		return SUCCESS;

	if (*s1 == NULL)
	{
		len_s1 = 0;
		if (!(*s1 = calloc(len_s1 + len_s2 + 1, sizeof(char))))
			return FAIL_MEMORY;
	}
	else
	{
		len_s1 = strlen(*s1);
		if (!(t = realloc(*s1, (len_s1 + len_s2 + 1) * sizeof(char))))
		{
			free(*s1);
			return FAIL_MEMORY;
		}
		else
			*s1 = t;
	}

	strcat(*s1, s2);

	return SUCCESS;
}

size_t len_string(char **s)
{
	return strlen(*s);
}

size_t get_string(char **s, const char *display)
{
	char c;
	char cs[2];
	size_t count = 0;

	printf("%s", display);

	while ((c = fgetc(stdin)) != '\n')
	{
		cs[0] = c;
		cs[1] = 0;
		if (append_string(s, cs))
			break;

		count++;
	}

	return count;
}

int copy_string(char **s, char *s1)
{
	char *t;
	size_t len;

	if (s1 == NULL)
		return SUCCESS;

	if (strlen(s1) == 0)
		return SUCCESS;

	len = strlen(s1);

	if (*s == NULL)
	{
		if (!(*s = calloc(len + 1, sizeof(char))))
			return FAIL_MEMORY;
	}
	else
	{
		if (!(t = realloc(*s, len + 1)))
		{
			free(*s);
			return FAIL_MEMORY;
		}
		else
			*s = t;
	}

	strcpy(*s, s1);

	return SUCCESS;
}

int truncate_string(char **s, size_t len)
{
	char *t;

	if (*s == NULL)
		return SUCCESS;

	if (len >= strlen(*s))
		return FAIL;

	if (!(t = realloc(*s, (len + 2) * sizeof(char))))
	{
		free(*s);
		return FAIL_MEMORY;
	}
	else
		t[len + 1] = '\0';

	*s = t;

	return SUCCESS;
}

int sprintf_string(char **s, char *fmt, ...)
{
	char *t;
	size_t len;
	va_list args;

	va_start(args, fmt);
	len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (*s == NULL)
	{
		*s = malloc(len + 1);
		if (*s == NULL)
			return FAIL_MEMORY;
	}
	else
	{
		t = realloc(*s, len + 1);

		if (t == NULL)
		{
			free(*s);
			return FAIL_MEMORY;
		}
		else
			*s = t;
	}

	va_start(args, fmt);
	vsprintf(*s, fmt, args);
	va_end(args);

	return SUCCESS;
}

int replace_string(char **s, const char *oldW, const char *newW)
{
	char *str;
	size_t i, j, l, cnt = 0;
	char *r;
	size_t newWlen = strlen(newW);
	size_t oldWlen = strlen(oldW);

	if (*s == NULL)
		return FAIL;

	str = strdup(*s);
	l = strlen(str) + 1;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (strstr(&str[i], oldW) == &str[i])
		{
			cnt++;
			i += oldWlen;
		}
	}

	if (!(r = realloc(*s, (i + cnt * (newWlen - oldWlen) + 1) * sizeof(char))))
	{
		free(*s);
		return FAIL_MEMORY;
	}

	i = 0;
	j = 0;

	for (i = 0; i < l;)
	{
		if (strstr(str + i, oldW) == str + i)
		{
			if (newW[0] != '\0')
				strcpy(&r[i], newW);
			j += newWlen;
			i += oldWlen;
		}
		else
			r[j++] = str[i++];
	}

	r[j] = '\0';
	free(str);
	*s = r;

	return SUCCESS;
}

int wrap_string(char **s, size_t columns)
{
	int nextspace = 0;
	size_t l, w;

	char *t;

	if (*s == NULL)
		return FAIL;

	l = strlen(*s) + 2;
	t = (char *)realloc(*s, l * sizeof(char));

	if (t == NULL)
	{
		free(*s);
		return FAIL_MEMORY;
	}

	for (w = 1; w < l; w++)
	{
		if (w % columns == 0)
			nextspace = 1;

		if (t[w] == ' ')
		{
			if (nextspace == 1)
			{
				t[w] = '\n';
				nextspace = 0;
			}
		}
	}

	*s = t;

	return SUCCESS;
}

int sub_string(char **str, size_t s, size_t e)
{
	char *temp;
	size_t x;
	size_t l = strlen(*str);

	if (e <= s)
		return FAIL_NUMBER;
	if (s > l || e > l)
		return FAIL_NUMBER;

	if (!(temp = malloc((e - s + 2) * sizeof(char))))
		return FAIL_MEMORY;

	for (x = 0; x <= (e - s); x++)
	{
		temp[x] = (*str)[s + x];
	}

	temp[x] = '\0';

	*str = temp;

	return SUCCESS;
}

int left_string(char **str, size_t s)
{
	char *temp;
	size_t x;
	size_t l = strlen(*str);

	if (s > l)
		return FAIL_NUMBER;

	if (!(temp = malloc((s + 2) * sizeof(char))))
		return FAIL_MEMORY;

	for (x = 0; x <= s; x++)
	{
		temp[x] = (*str)[x];
	}

	temp[x] = '\0';

	*str = temp;

	return SUCCESS;
}

int right_string(char **str, size_t s)
{
	char *temp;
	size_t x;
	size_t l = strlen(*str);

	if (s > l)
		return FAIL_NUMBER;

	if (!(temp = malloc((s + 2) * sizeof(char))))
		return FAIL_MEMORY;

	for (x = 0; x <= s; x++)
	{
		temp[x] = (*str)[x + (l - s - 1)];
	}

	temp[x] = '\0';

	*str = temp;

	return SUCCESS;
}

void pause_for_enter(const char *display)
{
	char ch;

	printf("%s", display);

	while (1)
	{
		ch = fgetc(stdin);

		if (ch == '\n')
			break;
	}

	return;
}

/* Math Functions */

/* Used with FOR loops to properly handle fractional step values */
int float_less_than(double f1, double f2, double step)
{
	if (f1 > f2 + 1e-14)
		return 0;

	if ((f2 + step) - f1 > 1e-14)
		return 1;
	else
		return 0;
}

int string_to_double(const char *str, double *v)
{
	char *ptr;
	errno = 0;
	*v = strtod(str, &ptr);

	if (errno == ERANGE)
	{
		return FAIL_NUMBER;
	}

	if (str == ptr)
		return FAIL_NUMBER;

	return SUCCESS;
}

int string_to_int(const char *str, int *v)
{
	char *ptr;
	errno = 0;
	*v = (int)strtol(str, &ptr, 10);

	if (errno == ERANGE)
	{
		printf("\nNumber Overflow/Underflow Error!\n");
		return FAIL;
	}

	if (str == ptr)
	{
		printf("\nInvalid Number Conversion Error!\n");
		return FAIL;
	}

	return SUCCESS;
}

size_t int_to_string(char **s, int i)
{
	sprintf_string(s, "%d", i);
	return strlen(*s);
}

size_t double_to_string(char **s, double d, int digits)
{
	char *dlen = NULL;
	sprintf_string(&dlen, "%%0.%df", digits);
	sprintf_string(s, dlen, d);
	return strlen(*s);
}

double get_double(const char *display)
{
	char *buffer = NULL;
	double value;
	int rtn;

	while (1)
	{
		get_string(&buffer, display);
		rtn = string_to_double(buffer, &value);
		free(buffer);
		buffer = NULL;

		if (rtn == SUCCESS)
			return value;
	}
}

int get_int(const char *display)
{
	char *buffer = NULL;
	int value;
	int rtn;

	while (1)
	{
		get_string(&buffer, display);
		rtn = string_to_int(buffer, &value);
		free(buffer);
		buffer = NULL;

		if (rtn == EXIT_SUCCESS)
			return value;
	}
}

double deg_to_rad(double deg)
{
	return PI / 180.0 * deg;
}

double rad_to_deg(double rad)
{
	return rad * 180.0 / PI;
}

void d_swap(double *a, double *b)
{
	double t = *a;
	*a = *b;
	*b = t;
}

int d_partition(double arr[], int low, int high)
{
	double pivot = arr[high];

	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{

		if (arr[j] < pivot)
		{
			i++;
			d_swap(&arr[i], &arr[j]);
		}
	}
	d_swap(&arr[i + 1], &arr[high]);

	return (i + 1);
}

void d_sort(double arr[], int low, int high)
{
	if (low < high)
	{

		int pi = d_partition(arr, low, high);

		d_sort(arr, low, pi - 1);
		d_sort(arr, pi + 1, high);
	}
}

int array_sort_double(double arr[], int count)
{
	if (arr == NULL)
		return FAIL_PARAMETER;
	if (count < 2)
		return FAIL_PARAMETER;

	d_sort(arr, 0, count - 1);

	return SUCCESS;
}

void i_swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int i_partition(int arr[], int low, int high)
{
	int pivot = arr[high];

	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{

		if (arr[j] < pivot)
		{
			i++;
			i_swap(&arr[i], &arr[j]);
		}
	}
	i_swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void i_sort(int arr[], int low, int high)
{
	if (low < high)
	{

		int pi = i_partition(arr, low, high);

		i_sort(arr, low, pi - 1);
		i_sort(arr, pi + 1, high);
	}
}

int array_sort_int(int arr[], int count)
{
	if (arr == NULL)
		return FAIL_PARAMETER;
	if (count < 2)
		return FAIL_PARAMETER;

	i_sort(arr, 0, count - 1);

	return SUCCESS;
}

/* CSV Functions*/
int csv_parse(char ***array, char *str, size_t *number_of_fields)
{
	char *newStr = NULL;
	char currentCharacter;
	char **strArray = NULL;
	int quote = 0;
	size_t csvLength = strlen(str);
	int maxFieldCount = 2; /* Start with two fields as MAX */
	int *comma_positions = NULL;
	int *comma_temp = NULL;
	int currentField = 0;
	int cleanStringPosition = 0;
	int i;
	int startPosition = 0;
	int fieldLength;

	/* Allocate memory for the comma position array */
	if (!(comma_positions = calloc(1, 1 + sizeof(int) * maxFieldCount)))
	{
		return FAIL_MEMORY;
	}

	/* Allocate memory for "cleaned up" string the same size as the original string to guarantee that it is big enough */
	if (!(newStr = calloc(1, sizeof(char) * (csvLength + 1))))
	{
		return FAIL_MEMORY;
	}

	/* First pass through to record the correct comma positions */
	for (i = 0; i < csvLength; i++)
	{
		/* Get a single character and skip any control or garbage characters */
		if ((currentCharacter = str[i]) < 32)
			continue;

		/* Handle quotes, escapes and commas */
		switch (currentCharacter)
		{
		/* Check for escape character not inside quotes */
		case 92:
		{
			if (quote == 0)
			{
				/* Move ahead one character */
				i++;
				/* Keep the next good character and move to the next good character position*/
				newStr[cleanStringPosition++] = str[i];
				/* Move on to the next new character */
				continue;
			}

			break;
		}

		/* Check for quote and keep track of pairs */
		case 34:
		{
			/* Toggle the pair indicator */
			quote = 1 - quote;
			/* Skip the quote */
			continue;
		}

		/* Check for comma that is NOT inside quotes */
		case 44:
		{
			if (quote == 0)
			{
				/* Check to see if we need to grow our comma position array */
				if (currentField == maxFieldCount - 1)
				{
					/* Double in size each time */
					maxFieldCount *= 2;

					/* Allocate more memory for the array*/
					comma_temp = realloc(comma_positions, sizeof(int) * maxFieldCount);

					if (comma_temp == NULL)
					{
						return FAIL_MEMORY;
					}
					else
						comma_positions = comma_temp;
				}

				/* Keep track of the comma positions and move to the next field*/
				comma_positions[currentField++] = cleanStringPosition;
			}
		}
		}

		/* Keep the good characters and move to the next good character position  */
		newStr[cleanStringPosition++] = currentCharacter;
	}

	/* Make sure that clean string gets NULL terminator */
	newStr[cleanStringPosition] = 0;
	/* Make sure to mark the end of the string as a "comma" position so that the last field gets included in the array and include the last field */
	comma_positions[currentField++] = cleanStringPosition;
	/* Record the Total number of fields to return to the calling function */
	*number_of_fields = currentField;
	/* Allocate an array of pointers to chars, not actually allocating any strings themselves */
	strArray = malloc(sizeof(char *) * currentField);
	if (strArray == NULL)
		return FAIL_MEMORY;

	/* Copy the strings to the new string array */
	for (i = 0; i < currentField; i++)
	{
		/* Calculate length of the current field plus the Null Terminator*/
		fieldLength = comma_positions[i] - startPosition + 1;
		/* Replace the comma with a Null terminator */
		newStr[comma_positions[i]] = 0;
		/* Allocate memory for the current field */
		strArray[i] = malloc(sizeof(char) * fieldLength);
		if (strArray[i] == NULL)
			return FAIL_MEMORY;
		/* Copy the string into the new array */
		memcpy(strArray[i], newStr + startPosition, fieldLength);
		/* Move our start position to the next field */
		startPosition = comma_positions[i] + 1;
	}

	/* Clean up the dynamic arrays */
	if (comma_positions)
		(comma_positions);
	comma_positions = NULL;
	if (newStr)
		free(newStr);
	newStr = NULL;

	*array = strArray;
	/* Return the new array back to the calling function */

	return SUCCESS;
}

/*  Clean up an array of strings
	Input:  Array of strings
			Number of strings
	Return: none
*/
void cleanup_csv_strings(char **strArray, size_t numberOfStrings)
{
	int i;

	/* Free the individual strings */
	for (i = 0; i < numberOfStrings; i++)
	{
		free(strArray[i]);
		strArray[i] = NULL;
	}

	/* Once the strings themselves are freed, free the actual array itself */
	free(strArray);
	strArray = NULL;
}