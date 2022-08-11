/*	Final Project Praktikum Algoritma Lanjut dan Struktur Data
		"Toko Kue"	
	Anastasia Meza Hida Vignesvara - 124210070
	Athaya Rizqia Fitriani         - 124210071 
*/
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <conio.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#define true 1
#define false 0
#pragma GCC diagnostic ignored "-Wwrite-strings"
using namespace std;

FILE *fakun;
FILE *froti; //linked list
FILE *froti2;
FILE *fq; //queue
FILE *fs; //stack

struct data{
	char un[50];
	char pw[50];
}daftar,login,akun[100];

struct pesan{
	int tgl;
	int bln;
	int thn;
};

struct beli{
	char un[50];
	char nama[50];
	char alamat[100];
	char nohp[16];
	char roti[50];
	int jml;
	int harga;
	pesan waktu;
}pemesan,q[100],s[100],temp;

struct typeinfo{
	char id[10];
	char nama[50];
	char jenis[50];
	int harga;
}roti,file[100];

bool found,found2,ganda,skip;
bool menuvalid,tanggalvalid;
bool firstrun = true;
char *nama_bulan(int n){
	static char *bln[] = {"", "Januari", "Februari", "Maret", "April",
						  "Mei", "Juni", "Juli", "Agustus", "September",
						  "Oktober", "November", "Desember"};
	return bln[n];
}
char back;
string pass,user;
string nama,alamat,nohp,namaroti;
int hh,bb,tt;
int *ptgl;
int i=0;
int pdaftar,plogin,padmin;
int tambah;
int totalq,totals,found3;
int totalharga;
//Akun 
void daftarAkun();
void loginAkun();
//Menu User
void pesan();
void riwayat();
//Menu Admin
void menuAdmin();
//1. Linked List
typedef struct typenode *typeptr;
struct typenode{
	typeinfo info;
	typeptr next;
};
typeptr kepala, ekor;
void buatlist();
void sisipnode(typeinfo rincian);
void hapusnode(char id[10]);
void editnode(char id[10]);
void bacamaju();
int listkosong();
//2. Queue 
typedef struct typequeue *queue;
struct typequeue{
	beli info;
	queue next;
};
queue qdepan, qbelakang;
void buatqueue();
int queuekosong();
void enqueue(beli rincian);
void dequeue();
void cetakqueue();
//3. Stack
typedef struct typestack *pstack;
struct typestack{
	beli info;
	pstack next;
};
pstack stackakhir, stackawal;
void buatstack();
int stackkosong();
void push(beli rincian);
void pop();
void cetakstack();

void quickSort(int first, int last, beli rincian[]){ //ascending
	int low = first;
	int high = last;
	int mid = ((first+last)/2);
		while(low<=high){
			while(rincian[low].waktu.thn < rincian[mid].waktu.thn)
				low++;
			while(rincian[high].waktu.thn > rincian[mid].waktu.thn)
				high--;	
			while(rincian[low].waktu.bln < rincian[mid].waktu.bln && rincian[low].waktu.thn == rincian[mid].waktu.thn)
				low++;
			while(rincian[high].waktu.bln > rincian[mid].waktu.bln && rincian[high].waktu.thn == rincian[mid].waktu.thn)
				high--;
			while(rincian[low].waktu.tgl < rincian[mid].waktu.tgl && rincian[low].waktu.bln == rincian[mid].waktu.bln)
				low++;
			while(rincian[high].waktu.tgl > rincian[mid].waktu.tgl && rincian[high].waktu.bln == rincian[mid].waktu.bln)
				high--;
			if(low <= high){
				temp = rincian[low];
				rincian[low++] = rincian[high];
				rincian[high--] = temp;
			}
		}
	if(first < high) quickSort(first, high, rincian);
	if(low < last) quickSort(low, last, rincian);
}

void importData(){
	fq = fopen("antrean.txt","r"); i=0;
		while(fread(&q[i],sizeof(q[i]),1,fq)){
			i++;
		}totalq = i;
	fclose(fq);
}

void simpanSorting(){
	importData();
	quickSort(0, totalq-1, q);
	buatqueue();
	fq = fopen("antrean.txt","w");
	for(i=0;i<totalq;i++){
		fwrite(&q[i],sizeof(q[i]),1,fq);
		enqueue(q[i]);
	}
	fclose(fq);
}

void bubbleSort(beli rincian[], int total){ //descending
	int j;
	for(i=0;i<total;i++){
		for(j=i+1;j<total;j++){
			if(rincian[i].waktu.thn < rincian[j].waktu.thn){
				temp = rincian[i]; rincian[i] = rincian[j]; rincian[j] = temp;
			}
			if(rincian[i].waktu.bln < rincian[j].waktu.bln && rincian[i].waktu.thn == rincian[j].waktu.thn){
				temp = rincian[i]; rincian[i] = rincian[j]; rincian[j] = temp;
			}
			if(rincian[i].waktu.tgl < rincian[j].waktu.tgl && rincian[i].waktu.bln == rincian[j].waktu.bln){
				temp = rincian[i]; rincian[i] = rincian[j]; rincian[j] = temp;
			}
		}
	}
}

void seqSearch(beli rincian[], int total){ //berdasarkan akun
	int first2 = 0, first3 = 0;
	for(i=0;i<total;i++){
		if(strcmp(rincian[i].un,user.c_str())==0) //jika ditemukan
			found2 = true;
		if(found2){
			if(rincian == s && first3==0){  //untuk pesanan yang telah selesai 
				cout << "Pesanan telah Diterima : " << endl << endl; 
				first3++;
			}
			if(rincian == q && first2==0){ //untuk pesanan yang masih dalam antrean
				cout << "Masih dalam Proses : " << endl << endl; 
				first2++;
			}
			cout << "Tanggal Pemesanan : " << rincian[i].waktu.tgl
				 << " " << nama_bulan(rincian[i].waktu.bln) << " "
				 << rincian[i].waktu.thn << endl;
			cout << "Pesanan atas" << endl;
			cout << "Nama\t\t: " << rincian[i].nama << endl;
			cout << "Alamat\t\t: " << rincian[i].alamat << endl;
			cout << "No. Telepon\t: " << rincian[i].nohp << endl << endl;
			
			//temporary
			nama = rincian[i].nama; alamat = rincian[i].alamat; nohp = rincian[i].nohp;
			hh = rincian[i].waktu.tgl; bb = rincian[i].waktu.bln; tt = rincian[i].waktu.thn;
			totalharga = 0;
			cout << "Rincian Pembayaran" <<endl;
			cout << "===============================================\n";
			cout << "|    Nama Roti    |   Jumlah   |     Harga    |\n";
			while(strcmp(rincian[i].nama,nama.c_str())==0 && strcmp(rincian[i].alamat,alamat.c_str())==0 
				  && strcmp(rincian[i].nohp,nohp.c_str())==0 && rincian[i].waktu.tgl == hh
				  && rincian[i].waktu.bln == bb && rincian[i].waktu.thn == tt)
				  {
				  	cout << "+-----------------+------------+--------------+\n";
					cout<<"|"
						<<right<<setw(13)<<rincian[i].roti<<right<<setw(5)<<"|"
						<<right<<setw(7)<<rincian[i].jml<<right<<setw(6)<<"|" 
						<<right<<setw(10)<<rincian[i].harga<<right<<setw(5)<<"|"<<endl;
					totalharga += (rincian[i].harga*rincian[i].jml);
					i++;
				  }i--;
			cout << "===============================================\n";
			cout << "Total Pembayaran : " << totalharga+10000 <<endl<<endl<<endl;
			found3 = 8;
		}
		found2 = false;
	}
}

void first(){   //pertama kali running program
	if(firstrun){
		froti = fopen("roti.txt","r"); //membaca file roti
		if(froti!=NULL){
			while(fread(&file[i],sizeof(file[i]),1,froti)){
				sisipnode(file[i]); //menyisipkan menu dari file
			}
		}
		fclose(froti);
		
		fq = fopen("antrean.txt","r"); //membaca file antrean
		if(fq!=NULL){
			while(fread(&q[i],sizeof(q[i]),1,fq)){
				enqueue(q[i]); //menambahkan data dari file
			}
		}
		fclose(fq);
		
		fs=fopen("history.txt", "r"); //membaca file history
		if(fs!=NULL){
			while(fread(&s[i],sizeof(s[i]),1,fs)){
				push(s[i]); //menambahkan data dari file 
			}
		}
		fclose(fs);
		
		firstrun = false;
	}
}

void index(){ //menu utama 
	int pilih1;
	do{
		system("cls");
		cout << "==============================" << endl;
		cout << "|                            |" << endl;
		cout << "|        Metha Bakery        |" << endl;
		cout << "|                            |" << endl;
		cout << "==============================" << endl;
		cout << "1. Daftar Akun\n";
		cout << "2. Login\n";
		cout << "0. Exit\n";
		cout << "\nPilih Menu : "; cin >> pilih1; system("cls");
		switch(pilih1){
			case 1:
				daftarAkun();
				break;
			case 2:
				loginAkun();
				break;
			case 0:
				exit(0);
				break;
			default:
				cout << "Maaf menu tidak tersedia..." << endl;
				break;
		}
		if(pdaftar==1 || plogin==1 || padmin==1){
			cout << "\nLogout? (y/t) : "; 		
		}else cout << "\nKembali? (y/t) : "; 
		cin >> back;
	}while(back!='t' && back!='T');
}

void home(){ //menu user
	int pilih2;
	do{
		system("cls");
		cout << "==============================" << endl;
		cout << "|                            |" << endl;
		cout << "|        Metha Bakery        |" << endl;
		cout << "|                            |" << endl;
		cout << "==============================" << endl;
		cout << "1. Pesan\n";
		cout << "2. Riwayat Pesanan\n";
		cout << "3. Logout\n";
		cout << "0. Exit\n";
		cout << "Pilih : "; cin >> pilih2; system("cls");
		switch(pilih2){
			case 1:
				pesan();
				break;
			case 2:
				riwayat();
				break;
			case 3:
				index();
				break;
			case 0:
				exit(0);
				break;
			default:
				cout << "Maaf menu tidak tersedia..." << endl;
				break;
		}cout << "\nKembali? (y/t) : "; cin >> back;
	}while(back!='t' && back!='T' && pilih2!=3);
}

int main(){
	buatlist();
	buatqueue();
	buatstack();
	first();
	index();
}

//Akun
void password(char pw[50]){ 
	char p;
	string pass;
	while(p!='\r'){ //ketika p bukan enter
		p = getch(); //p membaca input dari keyboard
		if(p=='\b' && pass.size()!=0){ //jika b adalah backspace (hapus) dan pass tidak kosong
			cout << "\b \b"; 
			pass.erase(pass.size()-1,1); //menghapus 1 karakter pass yang paling akhir
			continue; //lanjut ke iterasi berikutnya
		} 
		//jika p merupakan angka atau huruf
		if(p>='0' && p<='9' || p>='a' && p<='z' || p>='A' && p<='Z'){
			pass = pass + p; //p akan ditambahkan pada pass
			cout << "*"; //dengan output bintang (*)
		}else //selain itu,
			continue; //lanjut ke iterasi berikutnya
	}
	strcpy(pw,pass.c_str()); //pass akan dicopy ke variabel pw 
}

void daftar_failed(){ //jika daftar gagal
	cout << "\nPendaftaran akun gagal, username telah digunakan!" << endl;
	cout << "Kembali ke halaman utama untuk mendaftar ulang!\n" << endl;
	ganda = true;
}

void daftarAkun(){
	cout << "==============================" << endl;
	cout << "|         Daftar Akun        |" << endl;
	cout << "==============================" << endl;
	do{
		ganda = false;
		cout << "Username : "; cin >> daftar.un;
		cout << "Password : "; password(daftar.pw);
		cout << endl << endl;
		cout << "Loading..." << endl; sleep(1);
		//jika akun = akun admin maka pendaftaran gagal
		if(strcmp(daftar.un,"admin")==0 && strcmp(daftar.pw,"metha")==0)
			daftar_failed(); 
		else{
			fakun = fopen("akun.txt","r"); //membaca file akun
			if(fakun!=NULL){ 
				while(fread(&akun[i],sizeof(akun[i]),1,fakun)) //membaca isi file akun
				{
					//jika username akun telah digunakan maka pendaftaran gagal
					if(strcmp(akun[i].un,daftar.un)==0){ 
						daftar_failed(); 
						break;
					}
				}
			}
			fclose(fakun); 
		}
	}while(ganda); 
	//error handling akun ganda
	if(!ganda){ 
		fakun = fopen("akun.txt","a"); //menambah data pada file akun
		akun[i] = daftar; 
		fwrite(&akun[i],sizeof(akun[i]),1,fakun); //menulis data
		fclose(fakun); 
		cout << "Pendaftaran akun berhasil !"<<endl; 
		pdaftar = 1; plogin = 0; padmin = 0;
		sleep(1); system("cls");
		home(); //masuk ke menu user 
	}
}

void loginAkun(){
	bool end = true;
	char dakun;
	found = false;
	cout << "==============================" << endl;
	cout << "|           Login            |" << endl;
	cout << "==============================" << endl;
	cout << "Username : "; cin >> login.un;
	cout << "Password : "; password(login.pw);
	cout << endl << endl;
	cout << "Loading..." << endl; sleep(1);
	
	//jika admin login maka masuk ke menuAdmin();
	if(strcmp(login.un,"admin")==0 && strcmp(login.pw,"metha")==0){
		padmin = 1; pdaftar = 0; plogin = 0;
		menuAdmin();
	}else{
		fakun = fopen("akun.txt","r"); //membaca file akun
		if(fakun!=NULL){
			while(fread(&akun[i],sizeof(akun[i]),1,fakun))
			{
				//jika username ditemukan
				if(strcmp(akun[i].un,login.un)==0){
					//jika password sesuai maka berhasil login 
					if(strcmp(akun[i].pw,login.pw)==0){
						found = true; 
						cout << "Anda berhasil login !" << endl;
					}else{ //jika password salah
						found = false;
						cout << "Login gagal ! Password salah !" << endl;
					}
					end = false;
				}
			}
			//jika akun ditemukan maka masuk ke menu user
			if(found){
				plogin = 1; pdaftar = 0; 
				sleep(1); system("cls");
				home();
			}
			padmin = 0;
		}
		fclose(fakun);
		if(end){
			cout << "Akun tidak tersedia !" << endl;
			//pilihan apabila ingin membuat akun
			cout << "\nIngin membuat akun? (y/t) : "; cin >> dakun;
			if(dakun=='y' || dakun=='Y'){
				system("cls");
				daftarAkun();
			}
		}
	}
}

//Menu User
void pesan(){
	time_t now = time(0);
	tm *ltm = localtime(&now); //waktu saat ini
	
	int tgl[3];
	bool tanggalvalid;
	typeptr bantu; bacamaju();
	if(!listkosong()){
		cout<<"\nINFO : Pesanan Anda akan dikirimkan 2 hari setelah tanggal pemesanan!\n" << endl;
		fq = fopen("antrean.txt","a");
		do{ //Error Handling Tanggal
			ptgl = tgl;
			cout << "\nTanggal Pemesanan (hh/bb/tttt) : ";
			for(int j=0;j<3;j++){
				cin >> tgl[j]; cin.ignore();
			}tanggalvalid = true;
			
			if(tgl[1] <= 0 && tgl[1] > 12 || tgl[0] <= 0 || tgl[2] <= 0) tanggalvalid = false;
			
			if(tgl[1] < 8 && (tgl[1]%2) == 1 && tgl[0] > 31) tanggalvalid = false;
			if(tgl[1] > 7 && (tgl[1]%2) == 0 && tgl[0] > 31) tanggalvalid = false;
			
			if(tgl[1] < 8 && (tgl[1]%2) == 0 && tgl[0] > 30) tanggalvalid = false;
			if(tgl[1] > 7 && (tgl[1]%2) == 1 && tgl[0] > 30) tanggalvalid = false;
			
			if(tgl[1] == 2 && (tgl[2]%4) == 0 && tgl[0] > 29) tanggalvalid = false;
			if(tgl[1] == 2 && (tgl[2]%4) != 0 && tgl[0] > 28) tanggalvalid = false;
			
			if(tgl[2] < 1900+ltm->tm_year || tgl[2] > 1900+ltm->tm_year) tanggalvalid = false;
			
			if(tgl[1] < 1+ltm->tm_mon) tanggalvalid = false;
			if(tgl[1] == 1+ltm->tm_mon && tgl[0] < ltm->tm_mday) tanggalvalid = false;
			if(!tanggalvalid)
				cout << "\nTanggal tidak valid !\n";
		}while(!tanggalvalid);
		
		cout<<"\nIsi Data Penerima\n"<<endl;
		cout<<"Nama Lengkap\t: "; cin.getline(pemesan.nama, sizeof(pemesan.nama));
		cout<<"Alamat\t\t: "; cin.getline(pemesan.alamat, sizeof(pemesan.alamat));
		cout<<"No. Telepon\t: "; cin.getline(pemesan.nohp, sizeof(pemesan.nohp));
		
		//temporary
		if(pdaftar==1) user = daftar.un; //baca akun jika masuk melalui menu daftar
		if(plogin==1) user = login.un; //baca akun jika masuk melalui menu login
		hh = *ptgl; bb = *(ptgl+1); tt = *(ptgl+2);
		nama = pemesan.nama; alamat = pemesan.alamat; nohp = pemesan.nohp;
		
		cout << "\nBerapa jenis roti yang ingin Anda pesan? : "; cin >> tambah;
		i=0; totalharga = 0;
		while(i<tambah){
			strcpy(pemesan.un,user.c_str());
			strcpy(pemesan.nama,nama.c_str());
			strcpy(pemesan.alamat,alamat.c_str());
			strcpy(pemesan.nohp,nohp.c_str());
			pemesan.waktu.tgl = hh; pemesan.waktu.bln = bb; pemesan.waktu.thn = tt;
			char id[10];
			
			do{ //Error Handling jika ID-Roti tidak sesuai
				cout << "\nID-Roti\t: "; cin >> id;
				bantu = kepala->next; menuvalid = false;
				while(bantu->next != ekor && strcmp(id,bantu->info.id)!=0)
					bantu = bantu->next;
				if(strcmp(id,bantu->info.id)==0)
					menuvalid = true;
				else if(bantu->next == ekor)
					cout << "Masukkan ID-Roti dengan benar !" << endl;
			}while(!menuvalid);
			
			cout<<"Jumlah\t: "; cin>>pemesan.jml;
			strcpy(pemesan.roti,bantu->info.nama);
			pemesan.harga = bantu->info.harga;
			totalharga += (pemesan.harga * pemesan.jml);
			if(i+1 == tambah){ 
				totalharga += 10000;
				cout << "\nTotal Harga yang harus Anda Bayar (Ditambah biaya ongkir Rp10.000): Rp";
				cout << totalharga << endl << endl;
				cout<<"===== TERIMA KASIH TELAH BERBELANJA DI METHA BAKERY ====="<<endl<<endl;
			}
			
			q[i] = pemesan;
			fwrite(&q[i],sizeof(q[i]),1,fq);
			enqueue(q[i]);
			i++;
		}
		fclose(fq);
		
		//struk
		i=0;
		ofstream ofs("struk.txt", ios::app);
		if(ofs.is_open()){
			ofs<<"==============================================="<<endl;
			ofs<<"Akun : "<<q[i].un<<endl;
			ofs<<"Riwayat pemesanan Anda : "<<endl;
			ofs<<"==============================================="<<endl;
			ofs<<"Tanggal pemesanan : "<<q[i].waktu.tgl<<" "<<nama_bulan(q[i].waktu.bln)<<" "<<q[i].waktu.thn<<endl;
			ofs<<"Pesanan Atas"<<endl;
			ofs<<"Nama : "<<q[i].nama<<endl;
			ofs<<"Alamat : "<<q[i].alamat<<endl;
			ofs<<"No. Telepon : "<<q[i].nohp<<endl<<endl;
			ofs<<"Roti yang dipesan : "<<endl;
				for(int t = 0;t<tambah;t++){
					ofs<<"Nama Roti : "<<q[t].roti<<endl;
					ofs<<"Jumlah    : "<<q[t].jml<<endl<<endl;
					i++;
				}
			ofs<<"Total Harga : "<<totalharga<<endl;
			ofs<<"==============================================="<<endl;
			ofs.close();
		}
		simpanSorting();
	}
}

void riwayat(){
	cout<<"==============================================="<<endl;
	cout<<"            Riwayat pembelian Anda             "<<endl;
	cout<<"==============================================="<<endl<<endl;
	if(pdaftar==1) user = daftar.un; //baca akun jika masuk melalui menu daftar
	if(plogin==1) user = login.un; //baca akun jika masuk melalui menu login
	importData();
	fs=fopen("history.txt", "r"); i = 0;
	while(fread(&s[i],sizeof(s[i]),1,fs)){
		i++;
	}totals = i;
	fclose(fs);	
	//bubleSort descending
	bubbleSort(q, totalq);
	bubbleSort(s, totals);
	//sequential Search 	
	seqSearch(s, totals);
	seqSearch(q, totalq);
	if(!found2 && found3!=8)
		cout << "Belum ada pembelian.." << endl;
}

//Menu Admin

//1. Linked List Roti
void buatlist(){
	kepala = (typenode *) malloc(sizeof(typenode));
	ekor = (typenode *) malloc(sizeof(typenode));
	kepala->info.harga = -9999999;
	kepala->next = ekor;
	ekor->info.harga = 9999999;
	ekor->next = NULL;
}

int listkosong(){
	if(kepala->next==ekor)
		return(true);
	else
		return(false);
}

void sisipnode(typeinfo rincian){
	typeptr NB, bantu;
	NB = (typenode *) malloc(sizeof(typenode));
	strcpy(NB->info.id, rincian.id);
	strcpy(NB->info.nama, rincian.nama);
	strcpy(NB->info.jenis, rincian.jenis);
	NB->info.harga = rincian.harga;
	bantu = kepala;
	while(rincian.harga > bantu->next->info.harga)
		bantu = bantu->next;
	NB->next = bantu->next;
	bantu->next = NB;
}

void hapusnode(char id[10]){
	typeptr hapus, bantu;
	if(listkosong())
		cout << "Menu Masih Kosong" << endl;
	else{
		bantu = kepala;
		while(bantu->next!=ekor && strcmp(id,bantu->next->info.id)!=0)
			bantu = bantu->next;
		if(strcmp(id,bantu->next->info.id)==0){
			hapus = bantu->next;
			bantu->next = hapus->next;
			free(hapus);
			//Update data roti
			cout << "Roti dengan ID " << id << " berhasil terhapus dari list..."<<endl;
			froti = fopen("roti.txt","w"); //membuat file roti
			bantu = kepala->next;
			while(bantu!=ekor){
				file[i] = bantu->info;
				fwrite(&file[i],sizeof(file[i]),1,froti); //menulis data roti ke dalam file
				bantu = bantu->next;
			}
			fclose(froti);
		}else
			cout << "ID Roti Tidak Ditemukan..."<<endl;
	}
}

void editnode(char id[10]){ //crud edit menu roti berdasarkan ID yang dipilih
	typeptr edit, bantu;
	if(listkosong())
		cout << "Menu Masih Kosong" << endl;
	else{
		bantu = kepala;
		while(bantu->next!=ekor && strcmp(id,bantu->next->info.id)!=0)
			bantu = bantu->next;
		if(strcmp(id,bantu->next->info.id)==0){
			edit = bantu->next;
			cout << "\nEdit Roti dengan ID " << id << endl; cin.ignore();
			cout << "Nama\t: "; cin.getline(edit->info.nama,sizeof(edit->info.nama));
			int j;
			do{ //Error Handling Jenis Roti
				cout << "Jenis (1)Basah (2)Kering : "; cin >> j;
				switch(j){
					case 1:
						strcpy(edit->info.jenis,"Basah");
						break;
					case 2:
						strcpy(edit->info.jenis,"Kering");
						break;
					default:
						cout << "Pilih angka sesuai pilihan!\n"<<endl; j=0;
						break;
				}
			}while(j==0);
			cout << "Harga\t: "; cin >> edit->info.harga;
			//Update data roti
			cout << "\nRoti dengan ID " << id << " berhasil di edit..."<<endl;
			bantu->next = edit;
			froti = fopen("roti.txt","w"); //membuat file roti
			bantu = kepala->next;
			while(bantu!=ekor){
				file[i] = bantu->info;
				fwrite(&file[i],sizeof(file[i]),1,froti); //menulis data roti ke dalam file
				bantu = bantu->next;
			}
			fclose(froti);
		}else
			cout << "ID Roti Tidak Ditemukan..."<<endl;
	}
}

void bacamaju(){
	typeptr bantu;
	if(listkosong())
		cout << "Menu Masih Kosong" << endl;
	else{
		bantu = kepala->next;
		cout << "====================================================================="<<endl;
		cout << "|                                                                   |"<<endl;
		cout << "|                            Menu Roti                              |"<<endl;
		cout << "|                                                                   |"<<endl;
		cout << "====================================================================="<<endl;
		cout << "---------------------------------------------------------------------\n";
		cout << "|    ID Roti    |    Nama Roti    |    Jenis Roti    |     Harga    |\n";
		while(bantu!=ekor){
			cout << "+---------------+-----------------+------------------+--------------+\n";
			cout<<"|"
				<<right<<setw(11)<<bantu->info.id<<right<<setw(5)<<"|"
				<<right<<setw(13)<<bantu->info.nama<<right<<setw(5)<<"|" 
				<<right<<setw(12)<<bantu->info.jenis<<right<<setw(7)<<"|" 
				<<right<<setw(10)<<bantu->info.harga<<right<<setw(5)<<"|"<<endl;	
			bantu = bantu->next;	
		}cout << "---------------------------------------------------------------------\n";
	}
}

void input(){
	int j;
	cout << "Nama\t: "; cin.getline(roti.nama,sizeof(roti.nama));
	do{ //Error Handling Jenis Roti
		cout << "Jenis (1)Basah (2)Kering : "; cin >> j;
		switch(j){
			case 1:
				strcpy(roti.jenis,"Basah");
				break;
			case 2:
				strcpy(roti.jenis,"Kering");
				break;
			default:
				cout << "Pilih angka sesuai pilihan!\n"<<endl; j=0;
				break;
		}
	}while(j==0);
	cout << "Harga\t: "; cin >> roti.harga;
	file[i] = roti;
	sisipnode(roti);
	fwrite(&file[i],sizeof(file[i]),1,froti);	
	fwrite(&file[i],sizeof(file[i]),1,froti2);
	i++; 
}

void inputRoti(){
	typeptr bantu;
	cout << "Jumlah Menu : "; cin >> tambah;
	i=0;
	froti = fopen("roti.txt","a");
	froti2 = fopen("roti2.txt","a");
	while(i<tambah){
		if(listkosong()){
			cout << "\nID-Roti\t: "; cin >> roti.id; cin.ignore();
			input();
		}
		else{
			do{
				bantu = kepala->next;
				cout << "\nID-Roti\t: "; cin >> roti.id; 
				while(bantu!=ekor){
					if(strcmp(bantu->info.id,roti.id)==0){
						cout << "ID Roti telah digunakan!"<<endl; 
						break;
					}else if(bantu->next==ekor){
						cin.ignore(); input();
						break;
					}else bantu = bantu->next;
				}
			}while(strcmp(bantu->info.id,roti.id)==0);
		}	
	}
	fclose(froti2);
	fclose(froti);
}

//2. Queue Pembelian
void buatqueue(){
	qdepan = (typequeue *) malloc(sizeof(typequeue));
	qdepan = NULL;
	qbelakang = qdepan;
}

int queuekosong(){
	if(qdepan == NULL)
		return(true);
	else
		return(false);
}

void enqueue(beli rincian){
	queue NQ;
	NQ = (typequeue *) malloc(sizeof(typequeue));
	strcpy(NQ->info.un, rincian.un);
	strcpy(NQ->info.nama, rincian.nama);
	strcpy(NQ->info.alamat, rincian.alamat);
	strcpy(NQ->info.nohp, rincian.nohp);
	strcpy(NQ->info.roti, rincian.roti);
	NQ->info.waktu.tgl = rincian.waktu.tgl;
	NQ->info.waktu.bln = rincian.waktu.bln;
	NQ->info.waktu.thn = rincian.waktu.thn;
	NQ->info.jml = rincian.jml;
	NQ->info.harga = rincian.harga;
	if(qdepan == NULL)
		qdepan = NQ;
	else 
		qbelakang->next = NQ;
	qbelakang = NQ;
	qbelakang->next = NULL;
}

void dequeue(){
	queue hapus, bantu;
	if(queuekosong())
		cout << "Antrean masih kosong !" << endl;
	else{
		bantu = qdepan; user = bantu->info.un;
		nama = bantu->info.nama; alamat = bantu->info.alamat; nohp = bantu->info.nohp;
		hh = bantu->info.waktu.tgl; bb = bantu->info.waktu.bln; tt = bantu->info.waktu.thn;
		while(bantu!=NULL && bantu->info.waktu.tgl == hh && bantu->info.waktu.bln == bb && bantu->info.waktu.thn == tt 
			  && strcmp(bantu->info.nama,nama.c_str())==0 && strcmp(bantu->info.alamat,alamat.c_str())==0
			  && strcmp(bantu->info.un,user.c_str())==0 && strcmp(bantu->info.nohp,nohp.c_str())==0){
			  	hapus = bantu;
			  	bantu = hapus->next;
			  	qdepan = bantu;
			  	//Masuk ke history
			  	fs = fopen("history.txt", "a");
			  	s[i]=hapus->info;
			  	fwrite(&s[i], sizeof(s[i]),1,fs);
			  	push(s[i]);
			  	fclose(fs);
			  	
			  	free(hapus);
			  }
		//Update file antrean
		if(qdepan==NULL){
			fq = fopen("antrean.txt","w");
		}else{
			bantu = qdepan;
			fq = fopen("antrean.txt","w");
			while(bantu!=NULL){
				q[i] = bantu->info;
				fwrite(&q[i],sizeof(q[i]),1,fq);
				bantu = bantu->next;
			}
		}
		fclose(fq);
	}
}

void cetakqueue(){
	queue bantu;
	if(queuekosong())
		cout << "Antrean masih kosong !" << endl;
	else{
		bantu = qdepan;
		cout << "========================================================================================================================\n";
		cout << "|                                                                                                                      |\n";
		cout << "|                                                  ANTREAN PEMBELIAN                                                   |\n";
		cout << "|                                                                                                                      |\n";
		cout << "========================================================================================================================\n";
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
		cout << "|  Tanggal Pemesanan  |        Nama        |               Alamat              |    No Telepon   |       Pesanan       |\n";
		do{
			skip = false;
			cout << "+---------------------+--------------------+-----------------------------------+-----------------+---------------------+\n";	
			cout<<"|  " 
				<<left<<setw(3)<<bantu->info.waktu.tgl<<setw(10)<<nama_bulan(bantu->info.waktu.bln)<<bantu->info.waktu.thn
				<<right<<setw(3)<<"|"<<setw(18)<<bantu->info.nama<<setw(3)<<"|"
				<<right<<setw(32)<<bantu->info.alamat<<setw(4)<<"|"
				<<right<<setw(15)<<bantu->info.nohp<<setw(2)<<" ";
			user = bantu->info.un; 
			nama = bantu->info.nama; alamat = bantu->info.alamat; nohp = bantu->info.nohp;
			hh = bantu->info.waktu.tgl; bb = bantu->info.waktu.bln; tt = bantu->info.waktu.thn;
			while(bantu!=NULL && bantu->info.waktu.tgl == hh && bantu->info.waktu.bln == bb && bantu->info.waktu.thn == tt 
				  && strcmp(bantu->info.nama,nama.c_str())==0 && strcmp(bantu->info.alamat,alamat.c_str())==0
				  && strcmp(bantu->info.un,user.c_str())==0 && strcmp(bantu->info.nohp,nohp.c_str())==0){
				  	froti2 = fopen("roti2.txt","r");
				  	while(fread(&file[i],sizeof(file[i]),1,froti2)){
				  		if(strcmp(file[i].nama,bantu->info.roti)==0){
							namaroti = file[i].nama;
							break;
						}
					}
				  	fclose(froti2);
				  	
				  	if(skip)
				  		cout<<"|"<<right<<setw(22)<<"|"<<setw(21)<<"|"<<setw(36)<<"|"<<setw(17)<<" ";
				  	cout<<left<<setw(4)<<"|"<<setw(3)<<bantu->info.jml<<setw(12)<<namaroti<<right<<setw(4)<<"|"<<endl;
				  	skip = true;
				  	bantu = bantu->next;
				  }
		}while(bantu!=NULL);
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
	}
}

//3. Stack History
void buatstack(){
	pstack NS;
	NS = (typestack *) malloc(sizeof(typestack));
	NS = NULL;
	stackawal = NS;
	stackakhir = NS;
}

int stackkosong(){
	if(stackawal==NULL)
		return(true);
	else
		return(false);
}

void push(beli rincian){
	pstack NS;
	NS = (typestack *) malloc(sizeof(typestack));
	strcpy(NS->info.un, rincian.un);
	strcpy(NS->info.nama,rincian.nama);
	strcpy(NS->info.alamat,rincian.alamat);
	strcpy(NS->info.nohp,rincian.nohp);
	strcpy(NS->info.roti,rincian.roti);
	NS->info.jml = rincian.jml;
	NS->info.harga = rincian.harga;
	NS->info.waktu.tgl = rincian.waktu.tgl;
	NS->info.waktu.bln = rincian.waktu.bln;
	NS->info.waktu.thn = rincian.waktu.thn;
	
	if (stackawal==NULL) 
		stackawal=NS;  
	else
		stackakhir->next=NS; 	
		
	stackakhir=NS;	
	stackakhir->next=NULL;
}

void pop(){
	pstack shapus, sbantu;
		
	if(stackkosong())
		cout << "Belum ada pesanan yang selesai !" << endl;
	else{
		sbantu = stackawal; 
		shapus = stackakhir;
		user = shapus->info.un;
		nama = shapus->info.nama; alamat = shapus->info.alamat; nohp = shapus->info.nohp;
		hh = shapus->info.waktu.tgl; bb = shapus->info.waktu.bln; tt = shapus->info.waktu.thn;
		while(stackakhir!=NULL && stackakhir->info.waktu.tgl == hh && stackakhir->info.waktu.bln == bb && stackakhir->info.waktu.thn == tt 
			  && strcmp(stackakhir->info.nama,nama.c_str())==0 && strcmp(stackakhir->info.alamat,alamat.c_str())==0
			  && strcmp(stackakhir->info.un,user.c_str())==0 && strcmp(stackakhir->info.nohp,nohp.c_str())==0){
				shapus=stackakhir;
				fq = fopen("antrean.txt","a");
				q[i] = shapus->info;
				fwrite(&q[i],sizeof(q[i]),1,fq);
				fclose(fq);
			  	if (shapus==stackawal){
					stackawal=NULL;
					break;
				}
				else {
					while(sbantu->next->next!=NULL) 
						sbantu=sbantu->next; 
					stackakhir=sbantu; 
					stackakhir->next=NULL; 
					
					free(shapus);
				}
				sbantu = stackawal;
			  }

		sbantu = stackawal;
		if(stackawal==NULL){
			fs = fopen("history.txt","w");
		}else{
			fs = fopen("history.txt","w");
			while(sbantu != NULL){
				s[i] = sbantu->info;
				fwrite(&s[i],sizeof(s[i]),1,fs);
				sbantu = sbantu->next;
			}
		}
		fclose(fs);
	}
}

void cetakstack(){
	if(stackkosong())
		cout << "Belum ada pesanan yang selesai !" << endl;
	else{
		pstack sbantu;
		sbantu = stackawal;
		cout << "========================================================================================================================\n";
		cout << "|                                                                                                                      |\n";
		cout << "|                                                  HISTORY PENJUALAN                                                   |\n";
		cout << "|                                                                                                                      |\n";
		cout << "========================================================================================================================\n";
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
		cout << "|  Tanggal Pemesanan  |        Nama        |               Alamat              |    No Telepon   |       Pesanan       |\n";
		do{
			skip = false;
			cout << "+---------------------+--------------------+-----------------------------------+-----------------+---------------------+\n";	
			cout<<"|  " 
				<<left<<setw(3)<<sbantu->info.waktu.tgl<<setw(10)<<nama_bulan(sbantu->info.waktu.bln)<<sbantu->info.waktu.thn
				<<right<<setw(3)<<"|"<<setw(18)<<sbantu->info.nama<<setw(3)<<"|"
				<<right<<setw(32)<<sbantu->info.alamat<<setw(4)<<"|"
				<<right<<setw(15)<<sbantu->info.nohp<<setw(2)<<" ";
			user = sbantu->info.un; 
			nama = sbantu->info.nama; alamat = sbantu->info.alamat; nohp = sbantu->info.nohp;
			hh = sbantu->info.waktu.tgl; bb = sbantu->info.waktu.bln; tt = sbantu->info.waktu.thn;
			while(sbantu!=NULL && sbantu->info.waktu.tgl == hh && sbantu->info.waktu.bln == bb && sbantu->info.waktu.thn == tt 
				  && strcmp(sbantu->info.nama,nama.c_str())==0 && strcmp(sbantu->info.alamat,alamat.c_str())==0
				  && strcmp(sbantu->info.un,user.c_str())==0 && strcmp(sbantu->info.nohp,nohp.c_str())==0){
				  	froti2 = fopen("roti2.txt","r");
				  	while(fread(&file[i],sizeof(file[i]),1,froti2)){
				  		if(strcmp(file[i].nama,sbantu->info.roti)==0){
							namaroti = file[i].nama;
							break;
						}
					}
				  	fclose(froti2);
				  	
				  	if(skip)
				  		cout<<"|"<<right<<setw(22)<<"|"<<setw(21)<<"|"<<setw(36)<<"|"<<setw(17)<<" ";
				  	cout<<left<<setw(4)<<"|"<<setw(3)<<sbantu->info.jml<<setw(12)<<namaroti<<right<<setw(4)<<"|"<<endl;
				  	skip = true;
				  	sbantu = sbantu->next;
				  }
		}while(sbantu!=NULL);
		cout << "------------------------------------------------------------------------------------------------------------------------\n";
	}
}
	
void caripembeli(beli rincian[], int total){
	bool notfound = false;
	int first2 = 0;
	if(queuekosong()) 
		cout<<"\nBelum ada pembelian"<<endl;
		
	else{
		string namacari;
		cout<<"Masukkan nama pemesan : "; cin.ignore();
		getline(cin, namacari);
		
		for(i=0;i<total;i++){
			if(strcmp(rincian[i].nama,namacari.c_str())==0) //jika ditemukan
				found2 = true;
			if(found2){
				if(first2==0){
					cout<<"\nBerikut data yang Anda cari : "<<endl<<endl;
					first2++;
				}
				cout << "Tanggal Pemesanan : " << rincian[i].waktu.tgl
					 << " " << nama_bulan(rincian[i].waktu.bln) << " "
					 << rincian[i].waktu.thn << endl;
				cout << "Pesanan atas" << endl;
				cout << "Nama\t\t: " << rincian[i].nama << endl;
				cout << "Alamat\t\t: " << rincian[i].alamat << endl;
				cout << "No. Telepon\t: " << rincian[i].nohp << endl << endl;
				
				//temporary
				user = rincian[i].un;
				nama = rincian[i].nama; alamat = rincian[i].alamat; nohp = rincian[i].nohp;
				hh = rincian[i].waktu.tgl; bb = rincian[i].waktu.bln; tt = rincian[i].waktu.thn;
				totalharga = 0;
				cout << "Rincian Pembayaran" <<endl;
				cout << "===============================================\n";
				cout << "|    Nama Roti    |   Jumlah   |     Harga    |\n";
				while(strcmp(rincian[i].un,user.c_str())==0 && strcmp(rincian[i].nama,nama.c_str())==0 
					  && strcmp(rincian[i].alamat,alamat.c_str())==0 
					  && strcmp(rincian[i].nohp,nohp.c_str())==0 && rincian[i].waktu.tgl == hh
					  && rincian[i].waktu.bln == bb && rincian[i].waktu.thn == tt)
					  {
						cout << "+-----------------+------------+--------------+\n";
						cout<<"|"
							<<right<<setw(13)<<rincian[i].roti<<right<<setw(5)<<"|"
							<<right<<setw(7)<<rincian[i].jml<<right<<setw(6)<<"|" 
							<<right<<setw(10)<<rincian[i].harga<<right<<setw(5)<<"|"<<endl;
						totalharga += (rincian[i].harga*rincian[i].jml);
						i++;
					  }i--;
				cout << "===============================================\n";
				cout << "Total Pembayaran : " << totalharga+10000 <<endl<<endl<<endl;
			}
			if(!found2 && i+1==total) //Apabila pengecekan sudah selesai dan masih tidak ditemukan
				notfound = true; 
			found2 = false;
		}
		if(notfound)
			cout << "\nData tidak ditemukan..." << endl;
	}
}

void menuAdmin(){
	int pilih3;

	do{
		system("cls");
		cout << "=============================="<<endl;
		cout << "|                            |"<<endl;
		cout << "|           Admin            |"<<endl;
		cout << "|        Metha Bakery        |"<<endl;
		cout << "|                            |"<<endl;
		cout << "=============================="<<endl;
		cout << "1. Tambah Menu Roti\n"; 
		cout << "2. Lihat Menu Roti\n";
		cout << "3. Cari data pembelian\n";
		cout << "4. Lihat antrean pembelian\n";
		cout << "5. History Penjualan\n"; 
		cout << "6. Logout\n";
		cout << "0. Exit\n";
		cout << "Pilih : "; cin >> pilih3; system("cls");
		switch(pilih3){
			case 1:
				inputRoti();
				break;
			case 2:
				char edit;
				do{
					system("cls");
					bacamaju();
					if(!listkosong()){ //hapus list menu roti
						cout << "\nIngin hapus menu? (y/t) : "; cin >> back; 
						if(back=='y'){
							cout << "\nID Roti : "; cin >> roti.id;
							hapusnode(roti.id);
							sleep(3);
						}else{ //edit menu roti
							cout << "\nIngin edit menu? (y/t) : "; cin >> edit;
							if(edit=='y'){
								cout << "\nID Roti : "; cin >> roti.id;
								editnode(roti.id);
								sleep(3);
							}
						}
					}else
						break;
				}while(back=='y' || back=='Y' || edit=='y' || edit=='Y');
				break;
			case 3:
				importData();
				caripembeli(q, totalq);
				break;
			case 4:
				char cancel;
				int finish;
				do{
					system("cls");
					cetakqueue();
					if(!queuekosong()){
						cout << "\nApakah pesanan sudah selesai? [1] Sudah [2] Belum : "; cin >> finish;
						if(finish==1)
						{
							dequeue();
							cout << "\n1 Pesanan telah diselesaikan. Semangat untuk pesanan selanjutnya !" << endl;
							sleep(3);
						}
					}else
						break;
				}while(finish!=2);
				break;
			case 5:
				do{	
					system("cls");
					cetakstack();
					if(!stackkosong()){
						cout << "\nCancel ke Antrean? (y/t) : "; cin >> cancel;
						if(cancel=='y' || cancel=='Y'){
							pop();
							cout << "\nCancel Pesanan Berhasil !" << endl;
							simpanSorting();
							sleep(3);
							continue;
						}
					}else
						break;
				}while(cancel!='t' && cancel!='T');
				break;
			case 6:
				index();
				break;
			case 0:
				exit(0);
				break;
			default:
				cout << "Maaf menu tidak tersedia..." << endl;
				break;
		}cout << "\nKembali? (y/t) : "; cin >> back;
	}while(back!='t' && back!='T' && pilih3!=6);
}
