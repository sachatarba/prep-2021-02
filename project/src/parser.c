#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

typedef struct Header {
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
        if (buff[0] != ' ') {
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

int parser(const char *path_to_eml) {
    FILE* file = fopen(path_to_eml, "r");
    int size;
    Header** h = read_headers(file, &size);
    char *KEYS[] = {"From", "To", "Date", "Content-Type"};
    for (size_t current_KEY = 0; current_KEY < 4; ++current_KEY) {
        if (find_header(h, size, KEYS[current_KEY]) != NULL) {
            printf("|%s", find_header(h, size, KEYS[current_KEY]) -> value);
        }
    } 
    puts("1");
    /*for (int i = 0; i < size; ++i) {
        printf("|%s| -> |%s|\n", h[i]->key, h[i]->value);
    }
    */
    free_a(h, size);
    return 0;
}

