int main()
{
    int a, b;
    a = 2;
    b = 3;

    {
        putint(a + 2 + b + 1);

        int c;
        c = 5;
        b = c + 1;

        putint(a+b);
	}


	return a + b;
}
