#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// ===================== STRUCT & GLOBAL =====================
struct Barang {
    string kode, nama;
    double harga;
    int stok;
};

const int MAX = 100;
Barang daftar[MAX];
int n_barang = 0;
int jml_transaksi = 0;

// ===================== FUNGSI LOGIN =====================
bool login() {
    string user, pass;
    cout << "\n===== FAKHRI ELEKTRONIK =====\n";
    cout << "Username : "; cin >> user;
    cout << "Password : "; cin >> pass;
    return (user == "admin" && pass == "admin123");
}

// ===================== TAMBAH BARANG =====================
void tambahBarang() {
    if (n_barang >= MAX) {
        cout << "Data penuh!\n";
        return;
    }
    cout << "\n-- Tambah Barang --\n";
    cout << "Kode   : "; cin >> daftar[n_barang].kode;
    cout << "Nama   : "; cin.ignore(); getline(cin, daftar[n_barang].nama);
    cout << "Harga  : "; cin >> daftar[n_barang].harga;
    cout << "Stok   : "; cin >> daftar[n_barang].stok;
    n_barang++;
    cout << "Barang berhasil ditambahkan.\n";
}

// ===================== TAMPILKAN BARANG =====================
void tampilBarang() {
    cout << fixed << setprecision(0);
    cout << "\n-- Daftar Barang --\n";
    cout << left << setw(8) << "Kode"
         << setw(20) << "Nama"
         << setw(12) << "Harga"
         << "Stok\n";
    cout << string(50, '-') << "\n";
    for (int i = 0; i < n_barang; i++) {
        cout << left << setw(8) << daftar[i].kode
             << setw(20) << daftar[i].nama
             << setw(12) << daftar[i].harga
             << daftar[i].stok << "\n";
    }
}

// ===================== HAPUS BARANG =====================
void hapusBarang() {
    string kode;
    cout << "\n-- Hapus Barang --\n";
    cout << "Kode barang : "; cin >> kode;
    for (int i = 0; i < n_barang; i++) {
        if (daftar[i].kode == kode) {
            for (int j = i; j < n_barang - 1; j++)
                daftar[j] = daftar[j + 1];
            n_barang--;
            cout << "Barang berhasil dihapus.\n";
            return;
        }
    }
    cout << "Barang tidak ditemukan.\n";
}

// ===================== CETAK STRUK =====================
void cetakStruk(string items[], double subtotals[], int qty[], int jml,
                double total, double diskon, double total_akhir) {
    // Buat nama file berdasarkan nomor transaksi
    string fname = "struk_" + to_string(jml_transaksi + 1) + ".txt";

    ofstream f(fname);
    f << fixed << setprecision(0);
    f << "============================\n";
    f << "      FAKHRI ELEKTRONIK     \n";
    f << "============================\n";
    f << "Transaksi ke-" << jml_transaksi + 1 << "\n";
    f << "----------------------------\n";
    for (int i = 0; i < jml; i++) {
        f << left << setw(18) << items[i]
          << "x" << setw(3) << qty[i]
          << "Rp" << subtotals[i] << "\n";
    }
    f << "----------------------------\n";
    f << "Subtotal : Rp" << total << "\n";
    if (diskon > 0)
        f << "Diskon   : -Rp" << diskon << " (10%)\n";
    f << "TOTAL    : Rp" << total_akhir << "\n";
    f << "============================\n";
    f.close();
    cout << "Struk dicetak: " << fname << "\n";
}

// ===================== TRANSAKSI =====================
void transaksi() {
    string items[50];
    double subtotals[50];
    int qty[50];
    int jml = 0;
    double total = 0;
    char lagi;

    cout << fixed << setprecision(0);
    cout << "\n-- Transaksi Pembelian --\n";
    tampilBarang();

    do {
        string kode;
        int q;
        cout << "\nKode barang : "; cin >> kode;

        int idx = -1;
        for (int i = 0; i < n_barang; i++)
            if (daftar[i].kode == kode) { idx = i; break; }

        if (idx == -1) {
            cout << "Barang tidak ditemukan!\n";
        } else {
            cout << "Qty         : "; cin >> q;
            if (q > daftar[idx].stok) {
                cout << "Stok tidak cukup! (Stok: " << daftar[idx].stok << ")\n";
            } else {
                double sub = daftar[idx].harga * q;
                items[jml] = daftar[idx].nama;
                subtotals[jml] = sub;
                qty[jml] = q;
                jml++;
                total += sub;
                daftar[idx].stok -= q;
                cout << "Ditambahkan. Subtotal: Rp" << sub << "\n";
            }
        }
        cout << "Beli barang lagi? (y/n): "; cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');

    double diskon = 0, total_akhir = total;
    if (total > 500000) {
        diskon = total * 0.1;
        total_akhir = total - diskon;
        cout << "\n** Selamat! Anda mendapat diskon 10% **\n";
    }

    cout << "----------------------------\n";
    cout << "Subtotal : Rp" << total << "\n";
    if (diskon > 0)
        cout << "Diskon   : -Rp" << diskon << "\n";
    cout << "TOTAL    : Rp" << total_akhir << "\n";
    cout << "----------------------------\n";

    cetakStruk(items, subtotals, qty, jml, total, diskon, total_akhir);
    jml_transaksi++;
    cout << "Transaksi selesai. Total transaksi: " << jml_transaksi << "\n";
}

// ===================== MAIN =====================
int main() {
    // Login
    while (!login()) {
        cout << "Login gagal! Silakan coba lagi.\n";
    }
    cout << "\nLogin berhasil! Selamat datang, Admin.\n";

    // Menu utama
    int pilih;
    do {
        cout << "\n===== MENU UTAMA =====\n";
        cout << "1. Tambah barang\n";
        cout << "2. Hapus barang\n";
        cout << "3. Transaksi pembelian\n";
        cout << "4. Tampilkan daftar barang\n";
        cout << "5. Lihat jumlah transaksi\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: "; cin >> pilih;

        switch (pilih) {
            case 1: tambahBarang(); break;
            case 2: hapusBarang();  break;
            case 3:
                if (n_barang == 0) cout << "Belum ada barang!\n";
                else transaksi();
                break;
            case 4: tampilBarang(); break;
            case 5:
                cout << "Jumlah transaksi: " << jml_transaksi << "\n";
                break;
            case 0: cout << "Terima kasih. Sampai jumpa!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilih != 0);

    return 0;
}