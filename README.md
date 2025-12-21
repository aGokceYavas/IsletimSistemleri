# FreeRTOS PC Scheduler Simülasyonu

Bu proje, **Sakarya Üniversitesi İşletim Sistemleri** dersi kapsamında, FreeRTOS çekirdeğinin görev zamanlama mantığını PC (POSIX/Linux/WSL) ortamında simüle etmek amacıyla geliştirilmiştir.

Proje, FreeRTOS görev yapılarını kullanarak **4 seviyeli bir öncelikli sıralayıcı (scheduler)** algoritmasını uygular ve görevlerin durumlarını (çalışıyor, askıda, bitti) terminal üzerinde görselleştirir.

## Proje Ekibi

* **B221210371** - Aybüke Gökçe Yavaş
* **B221210047** - Pınar Ateşoğlu
* **B221210077** - Kaan Yılmaz
* **B241210355** - Melike Türkdal
* **B221210031** - Elif Yılmaz

---

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

---

## Kurulum ve Derleme

Proje **Linux** veya **Windows Subsystem for Linux (WSL)** ortamında çalıştırılmak üzere tasarlanmıştır.

### Ön Gereksinimler
Sisteminizde `gcc` derleyicisi ve `make` aracının yüklü olması gerekir. Yüklü değilse şu komutla kurabilirsiniz:

```bash
sudo apt update
sudo apt install build-essential

```

### Projeyi İndirme (Clone)

Terminali açın ve projeyi bilgisayarınıza çekin:

```bash
git clone https://github.com/aGokceYavas/IsletimSistemleri.git
cd IsletimSistemleri

```

### Derleme (Compile)

Proje klasörü içerisindeyken `make` komutunu kullanarak projeyi derleyin:

```bash
make

```

*Bu işlem sonucunda `freertos_sim` isimli çalıştırılabilir dosya oluşturulacaktır.*

---

## ▶️ Çalıştırma

Derleme işlemi hatasız tamamlandıktan sonra simülasyonu başlatmak için:

```bash
./freertos_sim

```

**Not:** Simülasyonun çalışması için aynı dizinde `giris.txt` dosyasının bulunması gerekmektedir.

---

## 📄 Girdi Dosyası Formatı (`giris.txt`)

Program, görevleri `giris.txt` dosyasından okur. Dosya formatı şu şekildedir:
`<Varış Zamanı>, <Öncelik>, <Çalışma Süresi>`

**Örnek:**

```text
0, 1, 5
1, 0, 2
3, 1, 4

```

---

## 🧹 Temizlik

Derleme sonucu oluşan dosyaları temizlemek için:

```bash
make clean

```
