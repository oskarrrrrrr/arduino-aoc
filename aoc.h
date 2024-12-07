#include <stddef.h>

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
