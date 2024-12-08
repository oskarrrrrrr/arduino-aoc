#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;

#ifdef AOC_2024_01

const int N = 1000;
int left[N];
int right[N];
int idx = 0;
int line_char = 0;

void reset() {
    for (int i = 0; i < N; i++) {
        left[i] = 0;
        right[i] = 0;
    }
    idx = 0;
    line_char = 0;
}

void on_byte(char byte) {
    if (byte == '\n') {
        idx++;
        line_char = 0;
    } else {
        if (line_char < 5) {
            left[idx] *= 10;
            left[idx] += byte - '0';
        } else if (8 <= line_char && line_char <= 12) {
            right[idx] *= 10;
            right[idx] += byte - '0';
        }
        line_char++;
    }
}

int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void solve(int *part1, int *part2) {
    qsort(left, idx, sizeof(int), cmp);
    qsort(right, idx, sizeof(int), cmp);

    *part1 = 0;
    for (int i = 0; i < idx; i++) {
        *part1 += abs(left[i] - right[i]);
    }

    *part2 = 0;
    for (int i = 0; i < idx; i++) {
        int freq = 0;
        for (int j = 0; j < idx; j++) {
            freq += left[i] == right[j];
        }
        *part2 += left[i] * freq;
    }
}

#endif // AOC_2024_01

// =============================================================================

#ifdef AOC_2024_02

#define N 20
int arr[N];
size_t arr_len;

int safe1_count;
int safe2_count;

void reset_arr() {
    for (int i = 0; i < N; i++) {
        arr[i] = 0;
    }
    arr_len = 0;
}

void reset() {
    reset_arr();
    safe1_count = 0;
    safe2_count = 0;
}

typedef int (*SafeCondFun)(int, int);

int inc_cond(int prev, int curr) {
    return prev > curr && (prev - curr) <= 3;
}

int dec_cond(int prev, int curr) {
    return prev < curr && (curr - prev) <= 3;
}

int is_safe(int *arr, int idx, size_t len, int dampen, SafeCondFun cond) {
    for (size_t i = idx + 1; i < len; i++) {
        if (!cond(arr[i-1], arr[i])) {
            if (dampen > 0) {
                dampen--;
                return (
                    ((i <= 1 || cond(arr[i-2], arr[i])) && is_safe(arr, i, len, dampen, cond))
                    || ((i + 1 >= len || cond(arr[i-1], arr[i+1])) && is_safe(arr, i+1, len, dampen, cond))
                );
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void on_byte(char byte) {
    if (byte == '\n') {
        arr_len++;

        int inc, dec;
        // part 1
        inc = is_safe(arr, 0, arr_len, 0, inc_cond);
        dec = is_safe(arr, 0, arr_len, 0, dec_cond);
        safe1_count += inc || dec;
        // part 2
        inc = is_safe(arr, 0, arr_len, 1, inc_cond);
        dec = is_safe(arr, 0, arr_len, 1, dec_cond);
        safe2_count += inc || dec;

        reset_arr();
    } else if (byte == ' ') {
        arr_len++;
    } else {
        arr[arr_len] *= 10;
        arr[arr_len] += byte - '0';
    }
}

void solve(int *part1, int *part2) {
    *part1 = safe1_count;
    *part2 = safe2_count;
}

#endif // AOC_2024_02

// =============================================================================

#ifdef AOC_2024_03

int result1;
int result2;

int left_num;
int right_num;

int do_state;
char do_pattern[] = "do()";
int do_pattern_len = 4;
int do_pattern_idx;
char dont_pattern[] = "don't()";
int dont_pattern_len = 7;
int dont_pattern_idx;

typedef enum {
    PARSE_STATE_FUN,
    PARSE_STATE_LEFT_NUM,
    PARSE_STATE_COMMA,
    PARSE_STATE_RIGHT_NUM,
    PARSE_STATE_CLOSING_BRACE,
} ParseState;

ParseState parse_state;
int curr_state_len;

void set_parse_state(ParseState new_state) {
    parse_state = new_state;
    curr_state_len = 0;
}

void reset_parse_state() {
    left_num = 0;
    right_num = 0;
    set_parse_state(PARSE_STATE_FUN);
}

char pattern[] = "mul(";
int pattern_len = 4;

void reset() {
    result1 = 0;
    result2 = 0;
    left_num = 0;
    right_num = 0;
    do_state = 1;
    do_pattern_idx = 0;
    dont_pattern_idx = 0;
    parse_state = PARSE_STATE_FUN;
    curr_state_len = 0;
}

int is_number(char byte) {
    switch (byte) {
    case '1': case '2': case '3':
    case '4': case '5': case '6':
    case '7': case '8': case '9':
    case '0':
        return 1;
    }
    return 0;
}

void on_byte(char byte) {
    if (byte == do_pattern[do_pattern_idx]) {
        do_pattern_idx++;
        if (do_pattern_idx == do_pattern_len) {
            do_state = 1;
            do_pattern_idx = 0;
        }
    } else {
        do_pattern_idx = 0;
    }

    if (byte == dont_pattern[dont_pattern_idx]) {
        dont_pattern_idx++;
        if (dont_pattern_idx == dont_pattern_len) {
            do_state = 0;
            dont_pattern_idx = 0;
        }
    } else {
        dont_pattern_idx = 0;
    }

    switch (parse_state) {
    case PARSE_STATE_FUN:
        if (byte == pattern[curr_state_len]) {
            curr_state_len++;
            if (curr_state_len == pattern_len) {
                set_parse_state(PARSE_STATE_LEFT_NUM);
            }
        } else {
            reset_parse_state();
        }
        break;
    case PARSE_STATE_LEFT_NUM:
        if (is_number(byte)) {
            left_num *= 10;
            left_num += byte - '0';
            curr_state_len++;
            if (curr_state_len == 3) {
                set_parse_state(PARSE_STATE_COMMA);
            }
        } else if (byte == ',') {
            set_parse_state(PARSE_STATE_COMMA);
            on_byte(byte);
        } else {
            reset_parse_state();
        }
        break;
    case PARSE_STATE_COMMA:
        if (byte == ',') {
            set_parse_state(PARSE_STATE_RIGHT_NUM);
        } else {
            reset_parse_state();
        }
        break;
    case PARSE_STATE_RIGHT_NUM:
        if (is_number(byte)) {
            if (curr_state_len < 3) {
                right_num *= 10;
                right_num += byte - '0';
                curr_state_len++;
            } else {
                reset_parse_state();
            }
        } else if (byte == ')') {
            set_parse_state(PARSE_STATE_CLOSING_BRACE);
            on_byte(byte);
        } else {
            reset_parse_state();
        }
        break;
    case PARSE_STATE_CLOSING_BRACE:
        if (byte == ')') {
            int v = left_num * right_num;
            result1 += v;
            result2 += do_state * v;
        }
        reset_parse_state();
        break;
    }
}

void solve(int *part1, int *part2) {
    *part1 = result1;
    *part2 = result2;
}

#endif // AOC_2024_03

// =============================================================================

#ifdef AOC_2024_04

typedef enum { X, M, A, S } XMAS;
#define XMAS_SIZE 2 // (bits)

#define CELL_COUNT 4000
#define CELL_SIZE (sizeof(int))
#define CELL_CAPACITY (CELL_SIZE * 8 / XMAS_SIZE)

int text[CELL_COUNT];
size_t write_cursor;
size_t line_len;
size_t lines;

char XMAS_char(XMAS v) {
    switch (v) {
    case X: return 'X';
    case M: return 'M';
    case A: return 'A';
    case S: return 'S';
    }
    return '?';
}

void get_cell_and_offset(int row, int col, int *cell, int *offset) {
    int idx = (row * line_len) + col;
    *cell = idx / CELL_CAPACITY;
    *offset = idx % CELL_CAPACITY;
}

void set_in_cell(int *cell, int offset, XMAS v) {
    *cell &= (-1) | (0b11 << (2 * offset));
    *cell |= v << (2 * offset);
}

XMAS get_from_cell(int cell, int offset) {
    return (XMAS)((cell >> (2 * offset)) & 0b11);
}

void set_next(XMAS v) {
    int cell_num = write_cursor / CELL_CAPACITY;
    int *cell = &text[cell_num];
    int offset = write_cursor % CELL_CAPACITY;
    set_in_cell(cell, offset, v);
    write_cursor++;
}

XMAS get(size_t row, size_t col) {
    int cell, offset;
    get_cell_and_offset(row, col, &cell, &offset);
    XMAS v = get_from_cell(text[cell], offset);
    return v;
}

void reset() {
    write_cursor = 0;
    line_len = 0;
    lines = 0;
}

void on_byte(char byte) {
    switch (byte) {
    case 'X': set_next(X); break;
    case 'M': set_next(M); break;
    case 'A': set_next(A); break;
    case 'S': set_next(S); break;
    case '\n':
        if (line_len == 0) {
            line_len = write_cursor;
        }
        lines++;
    }
}

int in_bounds(int line, int col) {
    return (0 <= line && line < lines) && (0 <= col && col < line_len);
}

void translate(int *line, int *col, int vec[2]) {
    *line += vec[0];
    *col += vec[1];
}

int check_vec(int line, int col, int vec[2]) {
    if (get(line, col) != X) return 0;
    XMAS todo[3] = { M, A, S };
    for (int i = 0; i < 3; i++) {
        translate(&line, &col, vec);
        if (!in_bounds(line, col)) return 0;
        if (get(line, col) != todo[i]) return 0;
    }
    return 1;
}

int part1() {
    int vectors[8][2] = {
        { 1, 0 },
        { -1, 0 },
        { 0, 1 },
        { 0, -1 },
        { 1, 1 },
        { 1, -1 },
        { -1, 1 },
        { -1, -1 },
    };
    int count = 0;
    for (size_t line = 0; line < lines; line++) {
        for (size_t col = 0; col < line_len; col++) {
            for (int vec_idx = 0; vec_idx < 8; vec_idx++) {
                count += check_vec(line, col, vectors[vec_idx]);
            }
        }
    }
    return count;
}

int m_or_s(XMAS v) {
    return v == S || v == M;
}

int check_x(int line, int col) {
    XMAS center = get(line, col);
    if (center != A) return 0;
    if (
        !in_bounds(line-1, col-1)
        || !in_bounds(line-1, col+1)
        || !in_bounds(line+1, col-1)
        || !in_bounds(line+1, col+1)
    ) return 0;
    XMAS top_left = get(line - 1, col - 1);
    XMAS top_right = get(line - 1, col + 1);
    XMAS bottom_left = get(line + 1, col - 1);
    XMAS bottom_right = get(line + 1, col + 1);
    return (
        (m_or_s(top_left) && m_or_s(bottom_right) && top_left != bottom_right)
        && (m_or_s(top_right) && m_or_s(bottom_left) && top_right != bottom_left)
    );
}

int part2() {
    int count = 0;
    for (size_t line = 0; line < lines; line++) {
        for (size_t col = 0; col < line_len; col++) {
            count += check_x(line, col);
        }
    }
    return count;
}

void solve(int *p1, int *p2) {
    *p1 = part1();
    *p2 = part2();
}

#endif // AOC_2024_04

// =============================================================================

#ifdef AOC_2024_05

int reading_rules;
char prev_byte;

typedef struct { u8 prev; u8 next; } Rule;
Rule rules[2000];
size_t rules_count;

size_t current_rule;
size_t char_in_line;

u8 curr_update[100];
size_t curr_update_len;

int result1;
int result2;

void reset() {
    reading_rules = 1;
    prev_byte = 0;
    rules_count = 0;
    current_rule = 0;
    char_in_line = 0;
    curr_update_len = 0;
    result1 = 0;
    result2 = 0;
}

int find_err(size_t *i1, size_t *i2) {
    for (size_t i = 0; i < curr_update_len; i++) {
        for (size_t j = i+1; j < curr_update_len; j++) {
            for (size_t r = 0; r < rules_count; r++) {
                if (
                    rules[r].next == curr_update[i]
                    && rules[r].prev == curr_update[j]
                ) {
                    *i1 = i;
                    *i2 = j;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// TODO: this doesn't work on Arduino
void part1() {
    size_t i1, i2;
    if (!find_err(&i1, &i2)) {
        result1 += curr_update[curr_update_len/2];
    }
}

// TODO: this is too slow for Arduino
void part2() {
    int err_count = 0;
    size_t i, j;
    while (find_err(&i, &j)) {
        u8 temp = curr_update[i];
        curr_update[i] = curr_update[j];
        curr_update[j] = temp;
        err_count++;
    }
    if (err_count > 0) {
        result2 += curr_update[curr_update_len/2];
    }
}

void on_byte(char byte) {
    if (byte == '\n') {
        if (!reading_rules) {
            part1();
            // part2();
            curr_update_len = 0;
        }
        char_in_line = 0;
        if (prev_byte == '\n') {
            rules_count = current_rule;
            reading_rules = 0;
        }
    } else if (reading_rules) {
        Rule *rule = &rules[current_rule];
        switch (char_in_line) {
        case 0:
            rule->prev = byte - '0';
            rule->next = 0;
            break;
        case 1:
            rule->prev = (rule->prev * 10) + (byte - '0');
            break;
        case 3:
            rule->next = byte - '0';
            break;
        case 4:
            rule->next = (rule->next * 10) + (byte - '0');
            current_rule++;
            break;
        }
        char_in_line++;
    } else {
        if (byte == ',') {
            // skip
        } else if (prev_byte == ',' || prev_byte == '\n') {
            curr_update_len++;
            curr_update[curr_update_len - 1] = byte - '0';
        } else {
            curr_update[curr_update_len - 1] *= 10;
            curr_update[curr_update_len - 1] += byte - '0';
        }
    }
    prev_byte = byte;
}

void solve(int *p1, int *p2) {
    *p1 = result1;
    *p2 = result2;
}

#endif // AOC_2024_05

// =============================================================================
