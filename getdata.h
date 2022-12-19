


void getdata (FILE **input, int *size, char **buf);

int getsize (const char* fileinput);






void getdata (FILE **input, int *size, char **buf) {

    const char fileinput[] = "input.txt";
    *input = fopen (fileinput, "rb");

    *size = getsize (fileinput);

    *buf = (char *) calloc ((*size + 1), sizeof (char));
    fread (*buf, sizeof (char), *size, *input);

}



int getsize (const char* fileinput) {

    struct stat st = {0};

    stat (fileinput, &st);

    return st.st_size;
}
