#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DELTA_OF_BOUNDARY_BEGIN 9
#define MULTIPART "multipart"
#define BOUNDARY "boundary"
#define BUFF_SIZE 1024
#define HEADER_DELIMITER ':'
#define new(type) (type*) malloc(sizeof(type))
#define new_a(type, size) (type*) calloc(size, sizeof(type))
#define new_s(size) (char*) calloc(size, sizeof(char))
#define free_a(pointer, size) for (int i = 0; i < (size); free(pointer[i]), ++i); \
    free(pointer)

char* strip(char* source) {
    if (source == NULL) {
        return NULL;
    }
    int length = strlen(source);
    if (source[0] != ' ' && source[length - 1] != ' ') {
        return source;
    } 
    int start = 0;
    int end = length;
    for (int i = 0; i < length; ++i) {
        if (source[i] != ' ') {
            break;
        }
        start = i;
    }
    if (length - start == 1) {
        source[0] = '\0';
        return source;
    }
    for (int i = length - 1; i >= 0; --i) {
        if (source[i] != ' ') {
            break;
        }
        end = i;
    }
    for (int i = start + 1; i <= end; ++i) {
        source[i - start - 1] = source[i];
    }
    source[end - start - 1] = '\0';
    return source;
}

char* replace_char(char* source, char sym, char to_replace) {
    int length = strlen(source);
    for (int i = 0; i < length; ++i) {
        if (source[i] == sym) {
            source[i] = to_replace;
        }
    }
    return source;
}

char* remove_char(char* source, int pos) {
    int length = strlen(source);
    if (pos < 0 || pos >= length) {
        return NULL;
    }
    for (int i = pos; i < length - 1; ++i) {
        source[i] = source[i + 1];
    }
    source[length - 1] = '\0';
    return source;
}

char* remove_char_by_value(char* source, char to_remove) {
    char* ptr = strchr(source, to_remove);
    if (ptr == NULL) {
        return NULL;
    }
    return remove_char(source, ptr - source);
}

char* remove_all_char(char* source, char to_remove) {
    while (remove_char_by_value(source, to_remove) != NULL);
    return source;
}

typedef struct {
    char* key;
    char* value;
} Header;

Header* new_header(char* key, char* value) {
    Header* ret = new(Header);
    ret->key = key;
    ret->value = value;
    return ret;
}

void free_header(Header* header) {
    free(header->key);
    free(header->value);
    free(header);
}

int split_string(char* source, char delim, char** left, char** right) {
    char* split = strchr(source, delim);
    int len = strlen(source);
    if (split == NULL) {
        return 1;
    }
    int pos = split - source;
    char* tmp_left = new_s(pos + 1);
    tmp_left = strncpy(tmp_left, source, pos);
    if (tmp_left == NULL) {
        return 1;
    }
    char* tmp_right = new_s(len - pos);
    tmp_right = strcpy(tmp_right, source + pos + 1);
    if (tmp_right == NULL) {
        return 1;
    }
    *left = tmp_left;
    *right = tmp_right;
    return 0;
}

Header* header_from_string(char* source) {
    source[strlen(source) - 1] = '\0';
    char* key;
    char* value;
    if (split_string(source, HEADER_DELIMITER, &key, &value) != 0) {
        return NULL;
    }
    remove_all_char(value, '\n');
    remove_all_char(value, '\r');
    return new_header(strip(key), strip(value));
}

Header** add(Header** dest, int* size, Header* el) {
    Header** ret = (Header**)realloc(dest, sizeof(Header*) * (*size + 1));
    ret[*size] = el;
    *size += 1;
    return ret;
}

Header** read_headers(FILE* file, int* size) {
    Header** ret = new_a(Header*, 0);
    Header* tmp;
    int ret_size = 0;
    char buff[BUFF_SIZE];
    char raw_header[BUFF_SIZE * 4];
    raw_header[0] = '\0';
    fgets(buff, BUFF_SIZE, file);
    strcpy(raw_header, buff);
    while (fgets(buff, BUFF_SIZE, file) != NULL) {
        if (buff[0] == '\n') {
            tmp = header_from_string(raw_header);
            if (tmp == NULL) {
                *size = ret_size;
                return ret;
            }
            ret = add(ret, &ret_size, tmp);
            break;
        }
        if (!(buff[0] == ' ' || buff[0] == '\t')) {
            tmp = header_from_string(raw_header);
            if (tmp == NULL) {
                *size = ret_size;
                return ret;
            }
            ret = add(ret, &ret_size, tmp);
            strcpy(raw_header, buff);
        } else {
            strcat(raw_header, buff);
        }
    }
    *size = ret_size;
    return ret;
}

Header* find_header(Header** source, int size, char* key) {
    if (source == NULL || key == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        if (source[i] != NULL && strcmp(source[i]->key, key) == 0) {
            return source[i];
        }
    }
    return NULL;
}

int is_multipart(Header* source) {
    
    if (source == NULL) {
        return 0;
    }
    if (strstr(source -> value, MULTIPART) != NULL) {
        return 1;
    }
    return 0;
}

/*char* get_boundary(Header* source) {
    if (strstr(source, BOUNDARY) == NULL) {
        return NULL;
    }
    char* ptr_of_start_of_boundary;
    char* ret[100];
    int ind = 0;
    ptr_of_start_of_boundary = strstr(source, BOUNDARY) + DELTA_OF_BOUNDARY_BEGIN;
    if (ptr_of_start_of_boundary == '"' || ptr_of_start_of_boundary == "'") {
        ++ptr_of_start_of_boundary;
    }
    while (*ptr_of_start_of_boundary != '\t' || *ptr_of_start_of_boundary != '\n' || *ptr_of_start_of_boundary != '\r' \
          || *ptr_of_start_of_boundary != ' ' || *ptr_of_start_of_boundary != '"' || *ptr_of_start_of_boundary != '\'' \
                                                                                  || *ptr_of_start_of_boundary != ';') {

               ret[ind] = *ptr_of_start_of_boundary;
               ++ptr_of_start_of_boundary;
               ++ind;
           }
    ret[ind] = '\0';
    return ret;
}
*/
char* get_boundary(Header source) {
    char* boundary;
    char* left;
    /*
    split_string(source.value, ';', &left, &boundary);
    split_string(source.value, ';', &left, &boundary);
    split_string(source.value, ';', &left, &boundary);
    split_string(boundary, '=', &left, &boundary);
    split_string(boundary, '=', &left, &boundary);
    split_string(boundary, '=', &left, &boundary);
    */
    char* ptr_boundary = strstr(source.value, BOUNDARY);
    split_string(ptr_boundary, '=', &left, &boundary);
    remove_all_char(boundary, '"');
    remove_all_char(boundary, '\'');
    remove_all_char(boundary, ';');
    return boundary;
}

int parts_counter(FILE* filename, char* boundary) {
    if (filename == NULL) {
        return -1;
    }
    size_t parts_counter = 0;
    //int current = 1;
    while (!feof(filename)) {
        char current_str[BUFF_SIZE];
        fgets(current_str, BUFF_SIZE, filename);
        if (current_str == NULL) {
            return -1;
        }
        remove_all_char(current_str, " ");
        remove_all_char(current_str, "\t");
        remove_all_char(current_str, "\n");
        remove_all_char(current_str, ";");
        if (strcmp(current_str, boundary)) {
            ++parts_counter;
            //printf("\n%i", current);
        }
        //++current;
    }
    return parts_counter - 2;
}

int parser(const char *path_to_eml) {
    FILE* file = fopen(path_to_eml, "r");
    //FILE* file = fopen("btests/emails/poorly-hardly-viable-crow2.eml", "r");
    int size;
    int parts = 1;
    Header** h = read_headers(file, &size);
    char* KEYS[] = {"From", "To", "Date"};
    for (size_t current_KEY = 0; current_KEY < 3; ++current_KEY) {
        if (find_header(h, size, KEYS[current_KEY]) != NULL) {
            printf("%s|", find_header(h, size, KEYS[current_KEY]) -> value);
        }
        else {
            printf("%c", '|');
        }
    } 
    //puts("1");
    fclose(file);
    //FILE* file1 = fopen("btests/emails/poorly-hardly-viable-crow2.eml", "r");
    FILE* file1 = fopen(path_to_eml, "r");
    Header* source = find_header(h, size, "Content-Type");
    if (is_multipart(source)) {
        //printf("%s", get_boundary(*source));
        parts = parts_counter(file1, get_boundary(*source));
    }
    printf("%i", parts);
    return 0;
    //printf("%s", find_header(h, size, KEYS[0]) -> value);
    //for (int i = 0; i < size; ++i) {
    //    printf("|%s| -> |%s|\n", h[i]->key, h[i]->value);
    }

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];

    parser(path_to_eml);
    return 0;
}
