#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* --- 1. Veri Yapıları --- */

// Hangi veri tipinin kullanıldığını takip etmek için enum
typedef enum {
    TYPE_FLOAT32 = 0,
    TYPE_INT8    = 1,   // Quantized (Sıkıştırılmış)
    TYPE_FLOAT16 = 2
} TensorType;

// Union yapısı
// Bu yapı sayesinde 'data' içindeki tüm işaretçiler aynı bellek adresini paylaşır.
// Bu, mikrodenetleyicilerde RAM tasarrufu sağlar.
typedef union {
    float* f32;
    int8_t* i8;
    uint16_t* f16;
} TensorStorage;

// Ana Tensor yapısı
typedef struct {
    int rows;
    int cols;
    TensorType type;
    TensorStorage data; // Union burada kullanılıyor
    float scale;        // Quantization (Sıkıştırma) katsayısı
} Tensor;

/* --- 2. Bellek Yönetimi Fonksiyonları --- */

// Yeni bir Tensor oluşturur ve bellekte yer ayırır (malloc)
Tensor* create_tensor(int rows, int cols, TensorType type) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    if (t == NULL) return NULL; // Bellek dolduysa hata döndür

    t->rows = rows;
    t->cols = cols;
    t->type = type;
    t->scale = 1.0f;

    int eleman_sayisi = rows * cols;

    // Tipine göre uygun büyüklükte bellek ayırıyoruz
    if (type == TYPE_FLOAT32) {
        t->data.f32 = (float*)malloc(eleman_sayisi * sizeof(float));
    } else if (type == TYPE_INT8) {
        t->data.i8 = (int8_t*)malloc(eleman_sayisi * sizeof(int8_t));
    } else {
        t->data.f16 = (uint16_t*)malloc(eleman_sayisi * sizeof(uint16_t));
    }

    return t;
}

// Belleği iade eder (free)
void free_tensor(Tensor* t) {
    if (t == NULL) return;

    // Union içindeki hangi alanın dolu olduğunu bilip onu serbest bırakmalıyız
    if (t->type == TYPE_FLOAT32) free(t->data.f32);
    else if (t->type == TYPE_INT8) free(t->data.i8);
    else if (t->type == TYPE_FLOAT16) free(t->data.f16);

    free(t);
}

/* --- 3. Quantization (Sıkıştırma) İşlemi --- */

// 32-bit Float verileri 8-bit Integer'a dönüştürür.
// Bu işlem modelin bellekte 4 kat daha az yer kaplamasını sağlar.
void quantize_tensor(Tensor* float_in, Tensor* int8_out, float scale) {
    if (float_in->type != TYPE_FLOAT32 || int8_out->type != TYPE_INT8) {
        printf("Hata: Tip uyusmazligi!\n");
        return;
    }

    int8_out->scale = scale;
    int toplam = float_in->rows * float_in->cols;

    for (int i = 0; i < toplam; i++) {
        // Matematiksel dönüşüm: int8 = float / scale
        int8_out->data.i8[i] = (int8_t)(float_in->data.f32[i] / scale);
    }
}

/* --- 4. Görüntüleme Fonksiyonları --- */

void print_tensor(Tensor* t, const char* baslik) {
    printf("\n=== %s ===\n", baslik);
    printf("Boyut: %d x %d | Veri Tipi: ", t->rows, t->cols);

    if (t->type == TYPE_FLOAT32) printf("FLOAT32 (Standart)\n");
    else if (t->type == TYPE_INT8) printf("INT8 (Sikistirilmis)\n");
    else printf("FLOAT16\n");

    for (int i = 0; i < t->rows; i++) {
        for (int j = 0; j < t->cols; j++) {
            int index = i * t->cols + j;
            if (t->type == TYPE_FLOAT32) {
                printf("%8.2f ", t->data.f32[index]);
            } else {
                printf("%8d ", t->data.i8[index]);
            }
        }
        printf("\n");
    }
}

/* --- 5. Ana Uygulama --- */

int main() {
    printf("Gomulu Sistem Muhendisi - Tensor Yonetim Paneli\n");
    printf("----------------------------------------------\n");

    // Senaryo: Bir sensorun topladığı 3x3lük float veriler
    int satir = 3, sutun = 3;
    float ham_veriler[] = {12.5, 45.0, 7.2, 100.1, 55.4, 2.0, 88.9, 10.0, 33.3};

    // 1. Orijinal Tensörü Oluştur ve Doldur
    Tensor* sensor_verisi = create_tensor(satir, sutun, TYPE_FLOAT32);
    for (int i = 0; i < satir * sutun; i++) {
        sensor_verisi->data.f32[i] = ham_veriler[i];
    }
    print_tensor(sensor_verisi, "SENSOR GIRIS VERISI");

    // 2. Quantization Uygula (Bellek tasarrufu için)
    // Scale (Ölçek) olarak 1.5 kullanalım
    Tensor* sikistirilmis_veri = create_tensor(satir, sutun, TYPE_INT8);
    quantize_tensor(sensor_verisi, sikistirilmis_veri, 1.5f);

    printf("\n[SISTEM]: Veriler Quantize edildi (Scale: 1.5)\n");
    print_tensor(sikistirilmis_veri, "SIKISTIRILMIS VERI (INT8)");

    // 3. Tip Dönüşümü ve Veri Analizi (Örnek Uygulama)
    // Sıkıştırılmış veriden orijinal veriyi tahmin etme
    printf("\n[ANALIZ]: Ilk eleman karsilastirmasi:\n");
    float tahmin = sikistirilmis_veri->data.i8[0] * sikistirilmis_veri->scale;
    printf("- Orijinal: %.2f\n", sensor_verisi->data.f32[0]);
    printf("- Tahmin  : %.2f\n", tahmin);

    // 4. Bellek Temizliği (Pointerlar serbest bırakılıyor)
    free_tensor(sensor_verisi);
    free_tensor(sikistirilmis_veri);

    printf("\nProgram basariyla sonlandi.\n");
    return 0;
}