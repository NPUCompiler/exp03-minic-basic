int main()
{
	int a, c;
    int b;
    a = 3;
    b = 2;
    {
        int c;
        c = a + b;
        b = c;
	}

	{
        int a, b;
        a = 4;
        b = 5;
        c = a + b;

	}

	return a + b + c;
}