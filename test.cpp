#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Ð? s? d?ng các hàm toán h?c nhu sin, cos, atan2, sqrt
#include <stdbool.h> // Ð? s? d?ng ki?u boolean trong C


#define PI 3.14159265358979323846
#define EARTH_RADIUS 6371.0  // Bán kính Trái Ð?t tính theo km
#define MAX_TINH 63          // S? lu?ng t?nh
#define VO_CUNG 1e9 

// Ð?nh nghia c?u trúc cho T?nh v?i tên và t?a d?
typedef struct Tinh {
    char ten[30];    // Tên c?a t?nh
    double vi_do;    // Vi d?
    double kinh_do;  // Kinh d?
} Tinh;

// Hàm chuy?n d?i t? d? sang radian
double do_sang_radian(double do_goc) {
    return do_goc * PI / 180.0;
}

// Hàm tính kho?ng cách gi?a hai t?nh d?a trên t?a d?
double tinhKhoangCach(Tinh* t1, Tinh* t2) {
    double delta_vi_do = do_sang_radian(t2->vi_do - t1->vi_do);
    double delta_kinh_do = do_sang_radian(t2->kinh_do - t1->kinh_do);

    double a = sin(delta_vi_do / 2) * sin(delta_vi_do / 2) +
               cos(do_sang_radian(t1->vi_do)) * cos(do_sang_radian(t2->vi_do)) *
               sin(delta_kinh_do / 2) * sin(delta_kinh_do / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

// Hàm kh?i t?o ma tr?n kho?ng cách gi?a các t?nh
void khoiTaoMaTranKhoangCach(double khoang_cach[MAX_TINH][MAX_TINH], int so_tinh) {
    for (int i = 0; i < so_tinh; i++) {
        for (int j = 0; j < so_tinh; j++) {
            khoang_cach[i][j] = 0;  // Kh?i t?o t?t c? kho?ng cách là 0
        }
    }
}

// Hàm thêm kho?ng cách gi?a các t?nh lân c?n vào ma tr?n
void themKhoangCachLanCan(double khoang_cach[MAX_TINH][MAX_TINH], Tinh tinh[], int index1, int index2) {
    double kc = tinhKhoangCach(&tinh[index1], &tinh[index2]);
    khoang_cach[index1][index2] = kc;
    khoang_cach[index2][index1] = kc; // Vì kho?ng cách là d?i x?ng
}

// Hàm hi?n th? ma tr?n kho?ng cách gi?a các t?nh
void hienThiMaTranKhoangCach(double khoang_cach[MAX_TINH][MAX_TINH], Tinh tinh[], int so_tinh) {
    printf("Ma tr?n kho?ng cách gi?a các t?nh:\n");
    printf("      ");
    for (int j = 0; j < so_tinh; j++) {
        printf("%-10s", tinh[j].ten);
    }
    printf("\n");

    for (int i = 0; i < so_tinh; i++) {
        printf("%-6s ", tinh[i].ten);
        for (int j = 0; j < so_tinh; j++) {
            printf("%-10.2f", khoang_cach[i][j]);
        }
        printf("\n");
    }
}

int timChiSoTinh(Tinh tinh[], int so_tinh, const char* ten_tinh) {
    for (int i = 0; i < so_tinh; i++) {
        if (strcmp(tinh[i].ten, ten_tinh) == 0) {
            return i;
        }
    }
    return -1; // Tr? v? -1 n?u không tìm th?y t?nh
}

void timDuongDiNganNhat(double khoang_cach[MAX_TINH][MAX_TINH], int so_tinh, int bat_dau, int ket_thuc, Tinh tinh[]) {
    double khoang_cach_min[MAX_TINH];
    bool da_tham[MAX_TINH];
    int truoc[MAX_TINH];

    // Kh?i t?o kho?ng cách ban d?u và dã tham
    for (int i = 0; i < so_tinh; i++) {
        khoang_cach_min[i] = VO_CUNG;
        da_tham[i] = false;
        truoc[i] = -1;
    }

    // Kho?ng cách t? di?m b?t d?u d?n chính nó là 0
    khoang_cach_min[bat_dau] = 0;

    // Thu?t toán Dijkstra
    for (int i = 0; i < so_tinh - 1; i++) {
        int minIndex = -1;
        double minDistance = VO_CUNG;

        // Tìm d?nh chua tham có kho?ng cách ng?n nh?t
        for (int j = 0; j < so_tinh; j++) {
            if (!da_tham[j] && khoang_cach_min[j] <= minDistance) {
                minDistance = khoang_cach_min[j];
                minIndex = j;
            }
        }

        // Ðánh d?u d?nh v?a tìm du?c là dã tham
        da_tham[minIndex] = true;

        // C?p nh?t kho?ng cách c?a các d?nh k? v?i d?nh v?a tìm du?c
        for (int k = 0; k < so_tinh; k++) {
            if (!da_tham[k] && khoang_cach[minIndex][k] != 0 &&
                khoang_cach_min[minIndex] != VO_CUNG &&
                khoang_cach_min[minIndex] + khoang_cach[minIndex][k] < khoang_cach_min[k]) {
                khoang_cach_min[k] = khoang_cach_min[minIndex] + khoang_cach[minIndex][k];
                truoc[k] = minIndex;
            }
        }
    }

    // In du?ng di ng?n nh?t t? `bat_dau` d?n `ket_thuc`
    if (khoang_cach_min[ket_thuc] == VO_CUNG) {
        printf("Khong co duong di tu %s den %s.\n", tinh[bat_dau].ten, tinh[ket_thuc].ten);
        return;
    }

    printf("Khoang cach ngan nhat tu %s den %s: %.2f km\n", tinh[bat_dau].ten, tinh[ket_thuc].ten, khoang_cach_min[ket_thuc]);

    // In ra các t?nh trên du?ng di
    printf("Duong di: ");
    int duong_di[MAX_TINH];
    int index = 0;
    for (int at = ket_thuc; at != -1; at = truoc[at]) {
        duong_di[index++] = at;
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%s", tinh[duong_di[i]].ten);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

// Hàm chính
int main() {
    // Kh?i t?o các d?i tu?ng t?nh
    Tinh tinh[MAX_TINH];
    int so_tinh = 10;

    // Kh?i t?o thông tin cho t?ng t?nh
    strcpy(tinh[0].ten, "Bac Kan");
    tinh[0].vi_do = 105.834828;
    tinh[0].kinh_do = 22.143653;

    strcpy(tinh[1].ten, "Thai Nguyen");
    tinh[1].vi_do = 105.82199;
    tinh[1].kinh_do = 21.567741;

    strcpy(tinh[2].ten, "Lang Son");
    tinh[2].vi_do = 106.758329;
    tinh[2].kinh_do = 21.853951;

    strcpy(tinh[3].ten, "Bac Giang");
    tinh[3].vi_do = 106.194082;
    tinh[3].kinh_do = 21.282073;
    
    strcpy(tinh[4].ten, "Ha Noi");
    tinh[4].vi_do = 105.83112601332833;
    tinh[4].kinh_do = 21.02882343518717;

    strcpy(tinh[5].ten, "Hung Yen");
    tinh[5].vi_do = 106.05576293526339;
    tinh[5].kinh_do = 20.65478643090547;

    strcpy(tinh[6].ten, "Thai Binh");
    tinh[6].vi_do = 106.332631;
    tinh[6].kinh_do = 20.447774;

    strcpy(tinh[7].ten, "Hai Duong");
    tinh[7].vi_do = 106.316359;
    tinh[7].kinh_do = 20.936819;

    strcpy(tinh[8].ten, "Hai Phong");
    tinh[8].vi_do = 106.683810;
    tinh[8].kinh_do = 20.845127;

    strcpy(tinh[9].ten, "Bac Ninh");
    tinh[9].vi_do = 106.071451;
    tinh[9].kinh_do = 21.177199;

    // T?o ma tr?n kho?ng cách
    double khoang_cach[MAX_TINH][MAX_TINH];
    khoiTaoMaTranKhoangCach(khoang_cach, so_tinh);

    // Thêm kho?ng cách gi?a các t?nh lân c?n
    themKhoangCachLanCan(khoang_cach, tinh, 0, 1); 
    themKhoangCachLanCan(khoang_cach, tinh, 0, 2); 
    themKhoangCachLanCan(khoang_cach, tinh, 1, 0); 
    themKhoangCachLanCan(khoang_cach, tinh, 1, 2);
    themKhoangCachLanCan(khoang_cach, tinh, 1, 3); 
    themKhoangCachLanCan(khoang_cach, tinh, 1, 4); 
    themKhoangCachLanCan(khoang_cach, tinh, 2, 0); 
    themKhoangCachLanCan(khoang_cach, tinh, 2, 1); 
    themKhoangCachLanCan(khoang_cach, tinh, 2, 3); 
    themKhoangCachLanCan(khoang_cach, tinh, 3, 1); 
    themKhoangCachLanCan(khoang_cach, tinh, 3, 2); 
    themKhoangCachLanCan(khoang_cach, tinh, 3, 4);
    themKhoangCachLanCan(khoang_cach, tinh, 3, 7);
    themKhoangCachLanCan(khoang_cach, tinh, 3, 9);  
    themKhoangCachLanCan(khoang_cach, tinh, 4, 1); 
    themKhoangCachLanCan(khoang_cach, tinh, 4, 3); 
    themKhoangCachLanCan(khoang_cach, tinh, 4, 5);
    themKhoangCachLanCan(khoang_cach, tinh, 4, 9);  
    themKhoangCachLanCan(khoang_cach, tinh, 5, 4); 
    themKhoangCachLanCan(khoang_cach, tinh, 5, 6); 
    themKhoangCachLanCan(khoang_cach, tinh, 5, 7); 
    themKhoangCachLanCan(khoang_cach, tinh, 5, 9);  
    themKhoangCachLanCan(khoang_cach, tinh, 6, 5);
    themKhoangCachLanCan(khoang_cach, tinh, 6, 7);
    themKhoangCachLanCan(khoang_cach, tinh, 6, 8);  
    themKhoangCachLanCan(khoang_cach, tinh, 7, 3);
    themKhoangCachLanCan(khoang_cach, tinh, 7, 5);  
    themKhoangCachLanCan(khoang_cach, tinh, 7, 6); 
    themKhoangCachLanCan(khoang_cach, tinh, 7, 8);
    themKhoangCachLanCan(khoang_cach, tinh, 7, 9); 
    themKhoangCachLanCan(khoang_cach, tinh, 8, 6); 
    themKhoangCachLanCan(khoang_cach, tinh, 8, 7); 
    themKhoangCachLanCan(khoang_cach, tinh, 9, 3); 
    themKhoangCachLanCan(khoang_cach, tinh, 9, 4);  
    themKhoangCachLanCan(khoang_cach, tinh, 9, 5);
    themKhoangCachLanCan(khoang_cach, tinh, 9, 7);  
  
 

    // Hi?n th? ma 
	//tr?n kho?ng cách
    hienThiMaTranKhoangCach(khoang_cach, tinh, so_tinh);
    printf("\n");
    char ten_tinh1[30], ten_tinh2[30];
    printf("Nhap ten tinh dau tien: ");
    fgets(ten_tinh1, sizeof(ten_tinh1), stdin);
    ten_tinh1[strcspn(ten_tinh1, "\n")] = '\0';  // Xóa ký t? xu?ng dòng

    printf("Nhap ten tinh thu hai: ");
    fgets(ten_tinh2, sizeof(ten_tinh2), stdin);
    ten_tinh2[strcspn(ten_tinh2, "\n")] = '\0'; 
    // Tìm ch? s? c?a hai t?nh
    int bat_dau = timChiSoTinh(tinh, so_tinh, ten_tinh1);
    int ket_thuc = timChiSoTinh(tinh, so_tinh, ten_tinh2);


    // Ki?m tra xem hai t?nh có h?p l? không
    if (bat_dau == -1 || ket_thuc == -1) {
        printf("Ten tinh khong hop le. Vui long kiem tra lai.\n");
        return 1;
    }
    
    timDuongDiNganNhat(khoang_cach, so_tinh, bat_dau, ket_thuc, tinh);


    return 0;
}

