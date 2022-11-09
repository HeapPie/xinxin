#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROW 64
#define MAX_COL 16
#define MAX_WORD 64
#define DELIMS ","

typedef struct
{
        char head[MAX_COL][MAX_WORD];
        double rows[MAX_ROW][MAX_COL];
        int cnt_row,cnt_col;
        int cur_row,cur_col;
} Sheet;

void init_sheet(Sheet **shet);
void read_word(Sheet *shet,char *word);
void write_word(Sheet *shet,char *word);
void read_file(Sheet *shet,char *filename);
void write_file(Sheet *shet,char *filename);
int find_col(Sheet *shet,char *colname);
int new_col(Sheet *shet,char *colname);
void operation(Sheet *shet);

int main(int argc, char *argv [])
{
        Sheet *shet;
        init_sheet(&shet);
        //check args
        if (argc != 3)
        {
                fprintf(stderr,"Usage: %s [input file] [output file].\n",argv[0]);
                exit(EXIT_FAILURE);
        }
        //
       read_file(shet,argv[1]);
       operation(shet);
       write_file(shet,argv[2]);
}
 
void init_sheet(Sheet **shet)
{
        *shet = (Sheet *) malloc(sizeof(Sheet));
        (**shet).cur_row = (**shet).cur_col = (**shet).cnt_row = (**shet).cnt_col = 0;
}
void read_word(Sheet *shet,char *word)
{
        if (shet->cur_row == 0) sprintf(shet->head[shet->cur_col],"%s",word);
        else shet->rows[shet->cur_row - 1][shet->cur_col] = strtod(word,NULL);
        if (shet->cnt_row < shet->cur_row) shet->cnt_row = shet->cur_row;
        if (shet->cnt_col < shet->cur_col) shet->cnt_col = shet->cur_col;
}
void write_word(Sheet *shet,char *word)
{
        if (shet->cur_row == 0) sprintf(word,"%s",shet->head[shet->cur_col]);
        else sprintf(word,"%lf",shet->rows[shet->cur_row - 1][shet->cur_col]);
}
void read_file(Sheet *shet,char *filename)
{
       FILE *fp = NULL;
       char row[MAX_COL * (MAX_WORD + 2)];
       char *word = NULL;
       //open file
       if ((fp = fopen(filename,"r")) == NULL)
       {
                fprintf(stderr,"Can't open file %s.\n",filename);
                exit(EXIT_FAILURE);
       }
       //read
       while (fscanf(fp,"%[^\n]%*c",row) > 0)
       {
               word = strtok(row,DELIMS);
               while (word != NULL)
               {
                        read_word(shet,word);
                        shet->cur_col += 1;
                        word = strtok(NULL,DELIMS);
               }
               shet->cur_col = 0;
               shet->cur_row += 1;
       }
       //close file
       if (fclose(fp) != 0) fprintf(stderr,"Error in closing ile %s.\n",filename);
}
void write_file(Sheet *shet,char *filename)
{
        FILE *fp = NULL;
        char word[MAX_WORD];
        //open file
        if ((fp = fopen(filename,"w")) == NULL)
        {
                fprintf(stderr,"Can't open file %s.\n",filename);
                exit(EXIT_FAILURE);
       }
       //
       for (shet->cur_row = 0; shet->cur_row <= shet->cnt_row; shet->cur_row++)
       {
                for (shet->cur_col = 0; shet->cur_col <= shet->cnt_col; shet->cur_col++)
                {
                        write_word(shet,word);
                        fputs(word,fp);
                        if (shet->cur_col != shet->cnt_col) fputs(DELIMS,fp);
                }
                if (shet->cur_row != shet->cnt_row) fputs("\n",fp);
       }
       //close file
       if (fclose(fp) != 0) fprintf(stderr,"Error in closing ile %s.\n",filename);
}
int find_col(Sheet *shet,char *colname)
{
        for (shet->cur_col = 0; shet->cur_col <= shet->cnt_col; shet->cur_col++)
                if (strcmp(shet->head[shet->cur_col],colname) == 0) return shet->cur_col;
        return -1;
}
int new_col(Sheet *shet,char *colname)
{
        if (find_col(shet,colname) != -1)
        {
                fprintf(stderr,"Name %s exists.\n",colname);
                return -1;
        }
        shet->cur_col = shet->cnt_col += 1;
        sprintf(shet->head[shet->cur_col],"%s",colname);
        return shet->cur_col;
}
void operation(Sheet *shet)
{
        double *prow;
        int col1,col2,colnew;
        col1 = find_col(shet,"y1");
        col2 = find_col(shet,"y2");
        colnew = new_col(shet,"average");
        for (shet->cur_row = 1; shet->cur_row <= shet->cnt_row; shet->cur_row++)
        {
                prow = shet->rows[shet->cur_row - 1];
                prow[colnew] = (prow[col1] + prow[col2])/2;
        }
}
