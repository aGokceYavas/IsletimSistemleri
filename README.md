Sakarya Üniversitesi İşletim Sistemleri Dersinin Ödev Reposu.

# FreeRTOS PC Scheduler Simülasyonu

Bu proje, **İşletim Sistemleri** dersi kapsamında, FreeRTOS çekirdeğinin görev zamanlama mantığını PC (POSIX/Linux/WSL) ortamında simüle etmek amacıyla geliştirilmiştir[cite: 1, 4].

Proje, FreeRTOS görev yapılarını kullanarak **4 seviyeli bir öncelikli sıralayıcı (scheduler)** algoritmasını uygular ve görevlerin durumlarını (çalışıyor, askıda, bitti) terminal üzerinde görselleştirir.

## Proje Özellikleri ve Algoritma

Zamanlayıcı (Scheduler), `giris.txt` dosyasından okunan görevleri aşağıdaki kurallara göre yönetir:

### 1. Kuyruk Yapısı
Sistemde 4 farklı öncelik seviyesi bulunmaktadır:
* **Öncelik 0 (Gerçek Zamanlı / Real-Time):** En yüksek önceliğe sahiptir. **İlk Gelen İlk Çalışır (FCFS)** algoritmasına göre çalışır. Bu görevler tamamlanana kadar kesilmezler.
* **Öncelik 1 & 2 (Kullanıcı Görevleri):** Geri beslemeli (Feedback) kuyruk mantığıyla çalışır. Her göreve **1 saniyelik** zaman dilimi (quantum) tanınır. Süresi dolan görevin önceliği düşürülür (örn: 1 -> 2).
* **Öncelik 3 (En Düşük Öncelik):** En alt seviyedeki görevler **Round Robin (Çevrimsel Sıralı)** algoritması ile çalıştırılır.

### 2. Zaman Aşımı (Timeout)
* Sistemde 20 saniyeden uzun süre kalan (varış zamanından itibaren) herhangi bir görev, tamamlanmamış olsa dahi otomatik olarak sonlandırılır.

### 3. Performans Analizi
* Simülasyon sonunda her görev için **Geçen Süre (Turnaround Time)** ve **Bekleme Süresi (Waiting Time)** hesaplanarak bir performans tablosu ekrana yazdırılır.






