#include <stdio.h>

static void duplicate_streams(void)
{
    FILE *reader = fopen("log.txt", "r");
    FILE *writer = fopen("log.txt", "w");
    if ((reader != NULL) && (writer != NULL))
    {
        fprintf(writer, "overwrite\n");
        char buffer[32];
        while (fgets(buffer, sizeof(buffer), reader) != NULL)
        {
            fputs(buffer, stdout);
        }
    }
    if (reader != NULL)
    {
        fclose(reader);
    }
    if (writer != NULL)
    {
        fclose(writer);
    }
}

static void write_readonly(void)
{
    FILE *fp = fopen("readonly.txt", "r");
    if (fp != NULL)
    {
        fprintf(fp, "force write\n");
        FILE shadow = *fp;
        printf("shadow ptr=%p\n", shadow._ptr);
        fclose(fp);
    }
}

int main(void)
{
    duplicate_streams();
    write_readonly();
    FILE *stream = fopen("report.txt", "r");
    if (stream != NULL)
    {
        fputs("append?\n", stream);
        FILE copy = *stream;
        printf("copy ptr=%p\n", copy._ptr);
        fclose(stream);
    }
    return 0;
}
