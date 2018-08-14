///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra�tija.lt)
// Sintezatoriaus komponentas RateChange.dll
// Failas greitis.cpp
// Autorius dr. Gintaras Skersys
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

/*********************************************************
 * Global�s kiti kintamieji
 ********************************************************/

// Greitinimo koeficientas (kiek kart� turi pailg�ti signalas)
// TODO: nuskaityti is programos komandines eilutes parametru
//double greitinimo_koef = 1.49;

// numeris, �einantis � naujai sukuriam� signalo ir anotacij� fail� pavadinimus
int bandymo_numeris = 102;

// Scenarijaus numeris
// TODO: nuskaityti is programos komandines eilutes parametru
// 1. I�mesti/dubliuoti: skardiems vidurin� burbul�, dusliems interval� tarp 30 ir 50% garso.
// 2. Kaip 1, bet skard�iuosius sutrumpina maksimaliai
// 3. Visus trumpinti/ilginti vienodai (tiek kart�, kiek liepia greitinimo_koef)
// 4. Visus trumpinti/ilginti proporcingai, priklausomai nuo fonemos pavadinimo 
// (balsius labiausiai, kitus ma�iau, sprogstamuosius dar ma�iau, r nekeisti)
// 5. Stengtis pritempti iki tos fonemos ilgio vidurkio, priklausomai nuo scenarijaus5_koeficientas 
// [2015-01-12: nuo 7 versijos 5 scenarijus nebeveikia, nes funkcija nuskaityti_anotacijas() pakeista d�l to, kad pasikeit� anotacij� failo formatas]
const int scenarijus = 3;

// Scenarijaus 5 koeficientas i� intervalo [0,1]: 
// jei didelis (arti vieneto), tai stengsis patrumpinti/pailginti iki vidutinio ilgio, 
// jei ma�as (arti nulio) - ma�iau stengsis pasiekti vidutin� ilg�
// TODO: nuskaityti is programos komandines eilutes parametru
double scenarijaus5_koeficientas = 1;

// Tai veiksmas, kur� programa atlieka. Kitaip tariant, programos veikimo re�imas. 
// �iuo metu realizuoti keturi re�imai.
// 1. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) l�tinim�/greitinim� 
// pagal nurodyt� scenarij� su nurodytu greitinimo koeficientu
// 2. Atlieka duoto signalo failo (signalo_failo_pavadinimas ir kt.) l�tinim�/greitinim�
// pagal nurodyt� scenarij� su �vairiais greitinimo koeficientais 
// (j� pradin� ir galutin� reik�mes bei �ingsn� galima nustatyti funkcijoje testas, 
// rezultat� � failus ne�ra�o, tik i�veda � ekran� gaut� faktin� sul�t�jim�/pagreit�jim�)
// 3. Randa fonem� ilgi� vidurkius 
// (fonem� ilgi� katalogas nurodomas kintamajame "katalogas" funkcijoje rasti_fonemu_ilgiu_vidurkius ())
// 4. Konvertuoja visus nurodytus signalo failus su nurodytais greitinimo koeficientais. 
const int veiksmas = 2;

// skirtumas tarp to, koks plotis buvo panaudotas rezultatuose, ir koks duomenyse.
// Bus neigiamas, jei signalo rezultatas sutrump�jo, ir teigiamas, jei pailg�jo.
// Per tiek padid�s fonem� ilgiai
//int einamasis_postumis = 0;

//size_t fonemos_nr_debug = 0;

// jei ne nulis, i�veda papildom� informacij� apie programos darb�
// jei > 100, i�veda detali� informacij�
//int debuginam = 101;

/*********************************************************
 * Global�s signalo (duomen�) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
#ifdef USE_SIGNALAS
char signalo_failo_pavadinimas[256] = "db.raw";
#else
char signalo_failo_pavadinimas[256] = "db.m4a";
#endif

// garso signalo masyvas, i�skiriamas dinami�kai
short * signalas = NULL;
DecoderHandle hDecoder = NULL;
short * langas = NULL;
int langas_length = 0;

// garso signalo masyvo ilgis
size_t signalo_ilgis = 0;

/*********************************************************
 * Global�s naujojo signalo (rezultato) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
const char * naujo_signalo_failo_pavadinimas = "10m2e-new.16le";
const char * naujo_signalo_failo_pavadinimo_pradzia = "rez\\10m2e-new";
const char * naujo_signalo_failo_pavadinimo_pabaiga = ".16le";

/*********************************************************
 * Global�s fonem� (duomen�) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char fonemu_failo_pavadinimas[256] = "db_fon_weights.txt";

// fonem� pavadinim� masyvas
char ** fonemos = NULL;

// fonem� ilgi� masyvas
int * fonemu_ilgiai = NULL;

// fonem� kiekis (fonem� masyvo ir fonem� ilgi� masyvo ilgis)
size_t fonemu_kiekis = 0;

/*********************************************************
 * Global�s naujieji fonem� (rezultat�) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
const char * naujo_fonemu_failo_pavadinimas = "10m3_ilg-new.txt";
const char * naujo_fonemu_failo_pavadinimo_pradzia = "rez\\10m3_ilg-new";
const char * naujo_fonemu_failo_pavadinimo_pabaiga = ".txt";

// fonem� ilgi� masyvas
int * nauji_fonemu_ilgiai = NULL;

/*********************************************************
 * Global�s duomen� kintamieji, susij� su vidutiniu fonem� ilgiu (reikalingi kai kuriems scenarijams)
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru?
char vidutiniu_ilgiu_fonemu_failo_pavadinimas[256] = "vidutiniai_fonemu_ilgiai.txt";

// fonem� pavadinim� masyvas
char ** skirtingos_fonemos = NULL;

// fonem� ilgi� masyvas
int * vidutiniai_fonemu_ilgiai = NULL;

// fonem� kiekis (fonem� masyvo ir fonem� ilgi� masyvo ilgis)
size_t skirtingu_fonemu_kiekis = 0;
		
/*********************************************************
 * Global�s pik� (duomen�) kintamieji
 ********************************************************/

// TODO: nuskaityti pavadinima is programos komandines eilutes parametru
char piku_failo_pavadinimas[256] = "db_pik.txt";

// pik� masyvas (pikai[0] yra pirmojo piko vieta "signalas" masyve, t.y. masyvo "signalas" indeksas)
unsigned int * pikai = NULL;

// pik� kiekis (pik� masyvo ilgis)
size_t piku_kiekis = 0;

/*********************************************************
 * Burbulai
 ********************************************************/

// Euristika nustatys, kuriuos burbuliukus reikia pa�alinti ar dubliuoti. 
// J� s�ra�� pateiks �iame kintamajame "burbulai". 
// Kad nereik�t� pastoviai i�skirin�ti jam atminties, 
// �ia i�skiriame vien� kart� ir pastoviai naudojame.
// TODO: pastoviai tikrinti, ar nevir�ijo MAX_BURBULAI. Vir�ijus ka�k� daryti.
//struct burbulas burbulai[MAX_KEICIAMI_BURBULAI];

/*********************************************************
I�veda klaidos prane�im� � stderr.
 ********************************************************/
void klaida (char * klaidos_pranesimas)
{
	char pranesimas [1024];
	sprintf (pranesimas, "Klaida: %s", klaidos_pranesimas);
	spausdinti_loga (pranesimas);
}

/*********************************************************
U�tikrina, kad masyvo naujas_signalas ilgis yra ne ma�esnis, 
nei reikiamas_ilgis.

Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int pailginti_masyva_naujas_signalas (size_t reikiamas_ilgis, struct tkontekstas * kontekstas)
{
	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): reikia pailginti nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, reikiamas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// jei negalima ilginti - baigiam darb�
	if (kontekstas->galima_pailginti_naujas_signalas == 0)
		return -1;

	// siulomas_naujas_ilgis - naujas ilgis, jei pailgintume pagal REALLOC_COEFF
	size_t siulomas_naujas_ilgis = (size_t) (kontekstas->naujo_signalo_masyvo_ilgis * REALLOC_COEFF);

	// galutinis naujas ilgis bus didesnysis i� siulomas_naujas_ilgis ir reikiamas_ilgis
	size_t naujas_ilgis;
	if (siulomas_naujas_ilgis < reikiamas_ilgis)
		naujas_ilgis = reikiamas_ilgis;
	else
		naujas_ilgis = siulomas_naujas_ilgis;

	// i�skiriame atminties
	short * naujas_naujas_signalas 
		= (short *) realloc ((void *)kontekstas->naujas_signalas, naujas_ilgis*sizeof(short));

	// jei nepavyko i�skirti atminties, baigiam darb�
	if (naujas_naujas_signalas == NULL) {
		if (debuginam) {
			char pranesimas [1024];
			sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): nepavyko pailginti signalo nuo %d iki %d", 
				kontekstas->naujo_signalo_masyvo_ilgis, naujas_ilgis);
			klaida (pranesimas);
		}
		free (kontekstas->naujas_signalas);
		kontekstas->naujas_signalas = NULL;
		return -1;
	}

	// u�nuliname naujai i�skirt� (papildom�) atmint�
	for (size_t i = kontekstas->naujo_signalo_masyvo_ilgis; i < naujas_ilgis; i++)
		naujas_naujas_signalas[i] = 0;

	if (debuginam) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: pailginti_masyva_naujas_signalas(): sekmingai pailginta nuo %d iki %d", 
			kontekstas->naujo_signalo_masyvo_ilgis, naujas_ilgis);
		spausdinti_loga (pranesimas);
	}

	// �simename nauj�j� masyv� ir jo ilg�
	kontekstas->naujas_signalas = naujas_naujas_signalas;
	kontekstas->naujo_signalo_masyvo_ilgis = naujas_ilgis;

	return 0;
}

/*********************************************************
 * Hann lango kair� pus� (did�janti).
 * Nukopijuoja i� signalo masyvo nuo pradzia
 * � nauj�j� signalo masyv� nuo nauja_pradzia
 * burbulo kairi�j� pus� plo�io lango_plotis.
 *
 * nauja_pradzia gali b�ti ir neigiamas, nes tono keitimo atveju galime ra�yti prie� masyvo prad�i�.
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
********************************************************/
int Hann_lango_kaire_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann lang� tik tuo atveju, kai lango_plotis > 1 (kad neb�t� dalybos i� nulio ir kitoki� problem�)
	if (lango_plotis <= 1)
		return 0;

	// tikriname, ar reikia ilginti naujo signalo masyv�

	// nustatome, kokio ilgio tur�t� b�ti masyvas naujas_signalas
	// (gali b�ti ir neigiamas, jei nauja_pradzia < 0 ir |nauja_pradzia| > lango_plotis)
	int reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis vir�ija dabartin�
	if (reikiamas_ilgis > (int) kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv�
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// �ingsnio ilgis
	// TODO: i�siai�kinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

#ifndef USE_SIGNALAS
	if (lango_plotis * 8 > langas_length)
	{
		langas_length = lango_plotis * 8;
		langas = (short*) realloc(langas, langas_length * sizeof(short));
	}

	int res = DecoderDecodeSamples(hDecoder, sample_offset + pradzia, lango_plotis, (int8_t *) langas, langas_length * sizeof(short), NULL);
#endif
	// taikome Hann lang�
	for (int i=0; i < lango_plotis; i++)
	{
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
#ifdef USE_SIGNALAS
			(signalas [pradzia + i] * 0.5 * (1 - cos (zingsnis * i)));
		printf("%d %d Hk\n", i, signalas [pradzia + i] );
#else
			(langas [i] * 0.5 * (1 - cos (zingsnis * i)));
		printf("%d %d Hk\n", i, langas [i] );
#endif
	}

	return 0;
}

/*********************************************************
 * Hann lango de�in� pus� (ma��janti).
 * Nukopijuoja i� signalo masyvo nuo pradzia
 * � nauj�j� signalo masyv� nuo nauja_pradzia
 * burbulo de�in� pus� plo�io lango_plotis
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int Hann_lango_desine_puse (int pradzia, int nauja_pradzia, int lango_plotis, struct tkontekstas * kontekstas)
{
	// taikome Hann lang� tik tuo atveju, kai lango_plotis > 1 (kad neb�t� dalybos i� nulio ir kitoki� problem�)
	if (lango_plotis <= 1)
		return 0;

	
#ifndef USE_SIGNALAS
	if (lango_plotis * 8 > langas_length)
	{
		langas_length = lango_plotis * 8;
		langas = (short*) realloc(langas, langas_length * sizeof(short));
	}

	int res = DecoderDecodeSamples(hDecoder, sample_offset + pradzia, lango_plotis, (int8_t *) langas, langas_length * sizeof(short), NULL);
#endif

	// tikriname, ar reikia ilginti naujo signalo masyv�

	// nustatome, kokio ilgio tur�t� b�ti masyvas naujas_signalas
	size_t reikiamas_ilgis = (size_t) (nauja_pradzia + lango_plotis);

	// jei reikiamas ilgis vir�ija dabartin�
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv�
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko_pailginti == -1) return -1;
	}

	// �ingsnio ilgis
	// TODO: i�siai�kinti: gal zingsnis = pi / lango_plotis; ?
	double zingsnis = pi / (lango_plotis - 1);

	// taikome Hann lang�
	for (int i=0; i < lango_plotis; i++) {
		kontekstas->naujas_signalas [nauja_pradzia + i] += (short)
#ifdef USE_SIGNALAS
			(signalas [pradzia + i] * 0.5 * (1 + cos (zingsnis * i)));
		printf("%d %d Hd\n", i, signalas [pradzia + i]);
#else
			(langas [i] * 0.5 * (1 + cos (zingsnis * i)));
		printf("%d %d Hd\n", i, langas [i]);
#endif
	}

	return 0;
}

/*********************************************************
 * Spausdinti informacij� apie duotus burbulus
 ********************************************************/
void spausdinti_burbulus (struct burbulas * burbulai, int burbulu_sk)
{
	for (int i=0; i< burbulu_sk; i++, burbulai++)
	{
		char pranesimas [1024];
		sprintf (pranesimas, "%d:[pr: %d, vid: %d, pab: %d, kartai: %d][plotis: %d, kaire: %d, desine: %d]",
			i, burbulai->pradzia, burbulai->vidurys, burbulai->pabaiga, burbulai->kartai, 
			burbulai->pabaiga - burbulai->pradzia, burbulai->vidurys - burbulai->pradzia, burbulai->pabaiga - burbulai->vidurys);
		spausdinti_loga (pranesimas);
	}
}

/*********************************************************
 * tono auk��io keitimas: apdorojame pus� pirmo burbulo (dal�, kertan�i� fonemos rib�).
 * Laikoma, kad fonema skard�ioji, turi pakankamai pik� (piku_sk > 1) ir tono auk�t� keisti reikia.
 ********************************************************/
int kopijuoti_signala_pradzioj (struct tkontekstas * kontekstas)
{
	// randame pirmojo burbulo kairiosios dalies plot�.

	// pirmojo burbulo de�iniosios dalies plotis - kairioji dalis bus ne platesn�
	unsigned int burbulo_desines_plotis = pikai [kontekstas->pirmojo_piko_nr + 1] - pikai [kontekstas->pirmojo_piko_nr];
	
	// Formuojant ant fonemos ribos esant� burbul�, tur�ti omenyje, kad u� ribos gali visai neb�ti pik�. 
	// Algoritmas b�t� toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kra�tas, 
	// o jei ne, tai formuoti simetri�k� burbul� (t.y. imti burbulo kra�t� atstumu X nuo centro).

	// jei prie� tai yra pikas, ir jis nelabai toli nuo pirmojo fonemos piko, tai ir bus burbulo prad�ia. 
	// Jei ne, burbulo prad�ia bus nutolusi nuo burbulo centro (pirmojo fonemos piko) tokiu pat atstumu, kaip pabaiga.
	int burbulo_pradzia = 0;
	if (kontekstas->pirmojo_piko_nr > 0 && pikai [kontekstas->pirmojo_piko_nr]-pikai[kontekstas->pirmojo_piko_nr-1] <= burbulo_desines_plotis)
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr-1];
	else
		burbulo_pradzia = pikai[kontekstas->pirmojo_piko_nr] - burbulo_desines_plotis;

	// pirmojo burbulo kairiosios dalies plotis
	int burbulo_kaires_plotis = pikai[kontekstas->pirmojo_piko_nr] - burbulo_pradzia;
	
	// tono auk��io keitimas: apskai�iuojame, kiek d�l tono auk��io keitimo turi pasislinkti pikas
	// (jei neigiama reik�m�, slinksis � kair�, jei teigiama, � de�in�).
	// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio at�vilgiu
	int einamasis_postumis = (int) (burbulo_kaires_plotis * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
	
	// jei pavyko == -1, visk� stabdome, nes nepavyko i�skirti atminties
	int pavyko = 0;
	// Hann lango kair� pus� - prisumuojame pirmojo burbulo kairi�j� dal�
	pavyko = Hann_lango_kaire_puse (burbulo_pradzia, 
		kontekstas->einamasis_naujo_signalo_nr - (kontekstas->fonemos_pradzia - burbulo_pradzia) + einamasis_postumis, 
		burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einam�sias signal� masyv� reik�mes
	kontekstas->einamasis_naujo_signalo_nr += + burbulo_kaires_plotis + einamasis_postumis - (kontekstas->fonemos_pradzia - burbulo_pradzia);
	kontekstas->einamasis_signalo_nr = pikai [kontekstas->pirmojo_piko_nr];

	// perskai�iuojame einam�j� post�m�
	kontekstas->einamasis_postumis += einamasis_postumis;

	return 0;
}

/*********************************************************
 * tono auk��io keitimas: apdorojame pus� paskutinio burbulo (dal�, kertan�i� fonemos rib�).
 * Laikoma, kad fonema skard�ioji, turi pakankamai pik� (piku_sk > 1) ir tono auk�t� keisti reikia.
 ********************************************************/
int kopijuoti_signala_pabaigoj (struct tkontekstas * kontekstas)
{
	// randame paskutiniojo burbulo de�iniosios dalies plot�.

	// paskutiniojo piko numeris
	size_t paskutinio_piko_nr = kontekstas->pirmojo_piko_nr + kontekstas->piku_sk - 1;

	// paskutiniojo burbulo kairiosios dalies plotis - de�inioji dalis bus ne platesn�
	unsigned int burbulo_kaires_plotis = pikai [paskutinio_piko_nr] - pikai [paskutinio_piko_nr - 1];
	
	// Formuojant ant fonemos ribos esant� burbul�, tur�ti omenyje, kad u� ribos gali visai neb�ti pik�. 
	// Algoritmas b�t� toks: tegu X yra burbulo kitos dalies plotis. 
	// Jei atstumu <= X nuo burbulo centro yra kitas pikas (kitoje fonemoje), tai jis ir bus burbulo kra�tas, 
	// o jei ne, tai formuoti simetri�k� burbul� (t.y. imti burbulo kra�t� atstumu X nuo centro).

	// jei paskui yra pikas, ir jis nelabai toli nuo paskutinio fonemos piko, tai ir bus burbulo pabaiga. 
	// Jei ne, burbulo pabaiga bus nutolusi nuo burbulo centro (paskutinio fonemos piko) tokiu pat atstumu, kaip prad�ia.
	int burbulo_pabaiga = 0;
	if (paskutinio_piko_nr + 1 < piku_kiekis && pikai [paskutinio_piko_nr + 1]-pikai [paskutinio_piko_nr] <= burbulo_kaires_plotis)
		burbulo_pabaiga = pikai [paskutinio_piko_nr + 1];
	else
		burbulo_pabaiga = pikai [paskutinio_piko_nr] + burbulo_kaires_plotis;

	// paskutiniojo burbulo de�iniosios dalies plotis
	int burbulo_desines_plotis = burbulo_pabaiga - pikai [paskutinio_piko_nr];
	
	// jei pavyko == -1, visk� stabdome, nes nepavyko i�skirti atminties
	int pavyko = 0;
	// Hann lango de�in� pus� - prisumuojame paskutiniojo burbulo de�ini�j� dal�
	pavyko = Hann_lango_desine_puse (pikai [paskutinio_piko_nr], 
		kontekstas->einamasis_naujo_signalo_nr, 
		burbulo_desines_plotis, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;
	
	// atnaujiname einam�sias signal� masyv� reik�mes
	kontekstas->einamasis_naujo_signalo_nr += kontekstas->fonemos_pabaiga - pikai [paskutinio_piko_nr];
	kontekstas->einamasis_signalo_nr = kontekstas->fonemos_pabaiga;

	// perskai�iuojame einam�j� post�m� (�iuo atveju - nepasikei�ia)
	kontekstas->einamasis_postumis += 0;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa�iu ir atnaujiname einam�sias signal� masyv� indeks� reik�mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_keiciant_tono_auksti (size_t iki, struct tkontekstas * kontekstas)
{
	// nieko nedarome, jei n�ra k� kopijuoti
	if (iki <= kontekstas->einamasis_signalo_nr)
		return 0;

	// surandame einamojo piko (nuo kurio reik�s visk� kopijuoti) numer�
	unsigned int pradzios_piko_nr = kontekstas->pirmojo_piko_nr;
	while (pradzios_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pradzios_piko_nr] < kontekstas->einamasis_signalo_nr) 
		pradzios_piko_nr++;
	if (pikai [pradzios_piko_nr] != kontekstas->einamasis_signalo_nr) {
		// taip negali b�ti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 1 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// surandame paskutinio piko, iki kurio reik�s visk� kopijuoti, numer�
	unsigned int pabaigos_piko_nr = pradzios_piko_nr;
	while (pabaigos_piko_nr < kontekstas->pirmojo_piko_nr + kontekstas->piku_sk && pikai [pabaigos_piko_nr] < iki) 
		pabaigos_piko_nr++;
	if (pikai [pabaigos_piko_nr] != iki) {
		// taip negali b�ti
		if (debuginam) {
			spausdinti_loga ("!!! Internal error 2 in function kopijuoti_signala_keiciant_tono_auksti() !!!");
			spausdinti_konteksta (kontekstas);
		}
	}

	// kopijuojame tarpus tarp vis� pik� nuo pradzios_piko_nr iki pabaigos_piko_nr

	for (unsigned int piko_nr = pradzios_piko_nr; piko_nr < pabaigos_piko_nr; piko_nr++) {
		
		// jei pavyko == -1, visk� stabdome, nes nepavyko i�skirti atminties
		int pavyko = 0;

		// tarpas tarp piku piko_nr ir piko_nr+1
		int tarpas_tarp_piku = pikai [piko_nr+1] - pikai [piko_nr];

		// Hann lango de�in� pus�
		pavyko = Hann_lango_desine_puse (pikai [piko_nr], kontekstas->einamasis_naujo_signalo_nr, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;
		
		// tono auk��io keitimas: apskai�iuojame, kiek d�l tono auk��io keitimo turi pasislinkti pikas
		// (jei neigiama reik�m�, slinksis � kair�, jei teigiama, � de�in�).
		// Kitaip tariant, tiek pasikeis naujo signalo ilgis originalaus signalo ilgio at�vilgiu
		int einamasis_postumis = (int) (tarpas_tarp_piku * (kontekstas->tarpo_tarp_piku_didinimo_koef - 1));
		
		// Hann lango kair� pus�
		pavyko = Hann_lango_kaire_puse (pikai [piko_nr], 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_postumis, 
			tarpas_tarp_piku, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einam�sias signal� masyv� reik�mes
		kontekstas->einamasis_naujo_signalo_nr += tarpas_tarp_piku + einamasis_postumis;
		kontekstas->einamasis_signalo_nr = pikai [piko_nr+1];
		
		// perskai�iuojame einam�j� post�m�
		kontekstas->einamasis_postumis += einamasis_postumis;
		
	}

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa�iu ir atnaujiname einam�sias signal� masyv� indeks� reik�mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala_nekeiciant_tono_aukscio (size_t iki, struct tkontekstas * kontekstas)
{
	// tikriname, ar reikia ilginti naujo signalo masyv�

	// nustatome, kokio ilgio tur�t� b�ti masyvas naujas_signalas
	size_t reikiamas_ilgis = kontekstas->einamasis_naujo_signalo_nr;
	if (iki > kontekstas->einamasis_signalo_nr)
		reikiamas_ilgis += iki - kontekstas->einamasis_signalo_nr;

	// jei reikiamas ilgis vir�ija dabartin�
	if (reikiamas_ilgis > kontekstas->naujo_signalo_masyvo_ilgis) {
		// reikia ilginti naujo signalo masyv�
		int pavyko_pailginti = pailginti_masyva_naujas_signalas (reikiamas_ilgis, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko_pailginti == -1) return -1;
	}

#ifndef USE_SIGNALAS
	int len = iki - kontekstas->einamasis_signalo_nr;
	if (len * 8 > langas_length)
	{
		langas_length = len *  8;
		langas = (short*) realloc(langas, langas_length * sizeof(short));
	}

	int res = DecoderDecodeSamples(hDecoder, sample_offset + kontekstas->einamasis_signalo_nr, len, (int8_t *) langas, langas_length * sizeof(short), NULL);
#endif

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo prad�ios
	// (negalime kopijuoti su memcpy, nes prarasime jau ten esan�i� informacij�).
	// Tuo pa�iu ir atnaujiname einam�sias signal� masyv� reik�mes.
#ifdef USE_SIGNALAS
	for (; kontekstas->einamasis_signalo_nr < iki; 
			kontekstas->einamasis_signalo_nr++, kontekstas->einamasis_naujo_signalo_nr++)
		{
			kontekstas->naujas_signalas[kontekstas->einamasis_naujo_signalo_nr] += signalas[kontekstas->einamasis_signalo_nr];
			printf("%d %d cp\n", kontekstas->einamasis_signalo_nr, signalas[kontekstas->einamasis_signalo_nr]);
		}
#else
	for (int i = 0; i < len; 
			i++, kontekstas->einamasis_signalo_nr++, kontekstas->einamasis_naujo_signalo_nr++)
		{
			kontekstas->naujas_signalas[kontekstas->einamasis_naujo_signalo_nr] += langas[i];
			printf("%d %d cp\n", kontekstas->einamasis_signalo_nr,langas[i]);
		}
#endif
	//memcpy (kontekstas->naujas_signalas + kontekstas->einamasis_naujo_signalo_nr, 
	//	signalas + kontekstas->einamasis_signalo_nr, 
	//	(pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr) * sizeof (short));

	// atnaujiname einam�sias signal� masyv� reik�mes
	//kontekstas->einamasis_naujo_signalo_nr += pirmas_burbulas->pradzia - kontekstas->einamasis_signalo_nr;
	//kontekstas->einamasis_signalo_nr = pirmas_burbulas->pradzia;

	return 0;
}

/*********************************************************
Jei yra nenukopijuoto signalo,
nukopijuojame (prisumuojame) signalo duomenis prie naujo signalo iki 'iki'.
Tuo pa�iu ir atnaujiname einam�sias signal� masyv� indeks� reik�mes 
einamasis_signalo_nr, einamasis_naujo_signalo_nr.

Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int kopijuoti_signala (size_t iki, struct tkontekstas * kontekstas)
{
	if (kontekstas->keisti_tono_auksti)
		// tono auk��io keitimas: kopijuojame, keisdami tono auk�t�
		return kopijuoti_signala_keiciant_tono_auksti (iki, kontekstas);
	else
		// tiesiog kopijuojame (prisumuojame signal�)
		return kopijuoti_signala_nekeiciant_tono_aukscio (iki, kontekstas);
}

/*********************************************************
 * I�meta burbul� grandin�. 
 * Tiksliau, nukopijuoja duomenis i� signalo masyvo � nauj� masyv�, 
 * "i�mesdamas" (nenukopijuodamas) visus grandin�s burbulus.
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ismesti_burbulu_grandine (struct burbulas * pirmas_burbulas,
							   struct burbulas * paskutinis_burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, visk� stabdome, nes nepavyko i�skirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo prad�ios
	// Tuo pa�iu ir atnaujiname einam�sias signal� masyv� reik�mes.
	pavyko = kopijuoti_signala (pirmas_burbulas->pradzia, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: ismesti_burbulu_grandine(): einamasis_signalo_nr: %d, einamasis_naujo_signalo_nr: %d", 
			kontekstas->einamasis_signalo_nr, kontekstas->einamasis_naujo_signalo_nr);
		spausdinti_loga (pranesimas);
	}

	// pirmojo burbulo kair�s dalies plotis
	int pirmojo_burbulo_kaires_plotis 
		= pirmas_burbulas->vidurys - pirmas_burbulas->pradzia;

	// paskutiniojo burbulo de�in�s dalies plotis
	int paskutinio_burbulo_desines_plotis 
		= paskutinis_burbulas->pabaiga - paskutinis_burbulas->vidurys;

	// apdorojame i�mest�j� dal� - tai bus dviej� Hann lang� suma

	// Hann lango de�in� pus� - prisumuojame pirmojo burbulo kairi�j� dal�
	pavyko = Hann_lango_desine_puse (pirmas_burbulas->pradzia, kontekstas->einamasis_naujo_signalo_nr, 
		pirmojo_burbulo_kaires_plotis, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;

	// apskai�iuojame, koks bus "apdorojamos dalies" plotis
	// (t.y. tos dalies, kur sumuojame Hann langus.
	// Jei yra pik�, tai tas plotis parodo, kur bus kitas pikas).
	// TODO: �iuo metu plo�iu laikau pirmojo burbulo kairiosios dalies ir 
	// paskutiniojo burbulo de�iniosios dalies plo�i� vidurk�.
	int einamasis_plotis = (pirmojo_burbulo_kaires_plotis + paskutinio_burbulo_desines_plotis)/2;

	// tono auk��io keitimas: kei�iame viet�, kur bus kitas pikas (t.y. einamasis_plotis), 
	// priklausomai nuo tarpo tarp pik� didinimo koeficiento
	einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
	// Hann lango kair� pus� - prisumuojame paskutiniojo burbulo de�ini�j� dal�
	pavyko = Hann_lango_kaire_puse (paskutinis_burbulas->vidurys, 
		kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - paskutinio_burbulo_desines_plotis, 
		paskutinio_burbulo_desines_plotis, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;

	// atnaujiname einam�sias signal� masyv� reik�mes
	kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
	kontekstas->einamasis_signalo_nr = paskutinis_burbulas->pabaiga;

	// perskai�iuojame einam�j� post�m�
	kontekstas->einamasis_postumis += einamasis_plotis 
		- (paskutinis_burbulas->pabaiga - pirmas_burbulas->pradzia);

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: ismesti_burbulu_grandine(): einamasis_postumis: %d", kontekstas->einamasis_postumis);
		spausdinti_loga (pranesimas);
	}

	return 0;
}

/*********************************************************
 * Trumpina signal�, perskai�iuodamas visus reikiamus masyvus.
 *
 * Burbulai i�metimui turi b�ti pateikti sur��iuoti i� eil�s.
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int trumpinti_fonema (struct tkontekstas * kontekstas)
{
	// burbul� grandin�s (i� eil�s einan�i� burbul� sekos) apdorojimas 
	// skiriasi nuo pavieni� burbul� apdorojimo, 
	// tod�l turime rasti tokias grandines. 
	// Tiksliau, apdorosime duomenis burbul� grandin�mis, 
	// kurios gali b�ti ir tik vieno burbulo ilgio.

	// einamojo burbulo numeris
	int burbulo_nr = 0;

	// kol turime neapdorot� burbul�
	while (burbulo_nr < kontekstas->keiciamu_burbulu_sk)
	{
		// i�saugome pradin� grandin�s burbul�
		struct burbulas * pirmas_burbulas = kontekstas->burbulai+burbulo_nr;

		// ie�kome paskutinio grandin�s burbulo.

		// Burbulas nebus paskutinis burbul� grandin�s burbulas, jei:
		// 1) jis nebus i�vis paskutinis burbulas, ir
		while (burbulo_nr < kontekstas->keiciamu_burbulu_sk - 1 
		// 2) kitas burbulas eina i�kart u� jo
		// (Burbulas B eina i�kart u� burbulo A, jei B.pradzia == A.vidurys)
		// TODO: gal dar reikia tikrinti, ar B.vidurys == A.pabaiga? Ir k� daryti, jei ne?
			&& kontekstas->burbulai[burbulo_nr].vidurys == kontekstas->burbulai[burbulo_nr+1].pradzia)
			burbulo_nr++;

		// i�saugome paskutin� grandin�s burbul�
		struct burbulas * paskutinis_burbulas = kontekstas->burbulai+burbulo_nr;

		// i�metame burbul� grandin�
		int pavyko = ismesti_burbulu_grandine (pirmas_burbulas, paskutinis_burbulas, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;

		// einame prie naujo burbulo
		burbulo_nr++;
	}

	return 0;
}

/*********************************************************
 * Iterpia duot� burbul� tiek kart�, kiek jame nurodyta. 
 * Tiksliau, nukopijuoja duomenis i� signalo masyvo � nauj� masyv�, 
 * dubliuodamas duot� burbul� tiek kart�, kiek jame nurodyta.
 *
 * Gali b�ti, kad �terpiamo burbulo plotis = 0, ir kad j� �terpti reikia 0 kart� (dummy burbulas). 
 * Tokiu atveju vienintelis efektas bus toks, 
 * kad nukopijuos signalo duomenis iki pirmojo burbulo vidurio 
 * ir atnaujins einam�sias signal� masyv� reik�mes (to reikia ptk ilginimui).
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int iterpti_burbula (struct burbulas * burbulas, struct tkontekstas * kontekstas)
{
	// jei pavyko == -1, visk� stabdome, nes nepavyko i�skirti atminties
	int pavyko = 0;

	// jei yra nenukopijuoto signalo
	// nukopijuojame (prisumuojame) signalo duomenis iki pirmojo burbulo vidurio
	// Tuo pa�iu ir atnaujiname einam�sias signal� masyv� reik�mes.
	pavyko = kopijuoti_signala (burbulas->vidurys, kontekstas);
	// jei nepavyko, visk� stabdome
	if (pavyko == -1) return -1;

	// burbulo kair�s dalies plotis
	int burbulo_kaires_plotis 
		= burbulas->vidurys - burbulas->pradzia;

	// burbulo de�in�s dalies plotis
	int burbulo_desines_plotis 
		= burbulas->pabaiga - burbulas->vidurys;

	// tiek kart�, kiek nurodyta burbule, nukopijuojame j� � nauj� signal�
	for (int i = 0; i < burbulas->kartai; i++)
	{
		// apskai�iuojame, koks bus "apdorojamos dalies" plotis
		// (t.y. tos dalies, kur sumuojame Hann langus.
		// Jei yra pik�, tai tas plotis parodo, kur bus kitas pikas).
		// Jis tolygiai kinta nuo burbulo_kaires_plotis iki burbulo_desines_plotis.
		int einamasis_plotis 
			= ((burbulas->kartai - i) * burbulo_kaires_plotis 
			+ (i + 1) * burbulo_desines_plotis) / (burbulas->kartai + 1);

		// tono auk��io keitimas: kei�iame viet�, kur bus kitas pikas (t.y. einamasis_plotis), 
		// priklausomai nuo tarpo tarp pik� didinimo koeficiento
		einamasis_plotis = (int) (einamasis_plotis * kontekstas->tarpo_tarp_piku_didinimo_koef);
		
		// apdorojame �terpt�j� dal� - tai bus dviej� Hann lang� suma
		
		// Hann lango de�in� pus� - prisumuojame pirmojo burbulo kairi�j� dal�
		pavyko = Hann_lango_desine_puse (burbulas->vidurys, kontekstas->einamasis_naujo_signalo_nr, 
			burbulo_desines_plotis, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;
		
		// Hann lango kair� pus� - prisumuojame paskutiniojo burbulo de�ini�j� dal�
		pavyko = Hann_lango_kaire_puse (burbulas->pradzia, 
			kontekstas->einamasis_naujo_signalo_nr + einamasis_plotis - burbulo_kaires_plotis, 
			burbulo_kaires_plotis, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;
		
		// atnaujiname einam�sias signal� masyv� reik�mes
		kontekstas->einamasis_naujo_signalo_nr += einamasis_plotis;
		//kontekstas->einamasis_signalo_nr += 0; // nepasikei�ia
		
		// perskai�iuojame einam�j� post�m�
		kontekstas->einamasis_postumis += einamasis_plotis;
	}

	if (debuginam > 100) {
		char pranesimas [1024];
		sprintf (pranesimas, "greitis.cpp: iterpti_burbula(): einamasis_postumis: %d", kontekstas->einamasis_postumis);
		spausdinti_loga (pranesimas);
	}

	return 0;
}

/*********************************************************
 * Ilgina signal�, perskai�iuodamas visus reikiamus masyvus.
 *
 * Gr��ina 0, jei pavyko, ir -1, jei nepavyko.
 ********************************************************/
int ilginti_fonema (struct tkontekstas * kontekstas)
{
	for (int burbulo_nr=0; burbulo_nr < kontekstas->keiciamu_burbulu_sk; burbulo_nr++) {
		int pavyko = iterpti_burbula (kontekstas->burbulai+burbulo_nr, kontekstas);
		// jei nepavyko, visk� stabdome
		if (pavyko == -1) return -1;
	}

	return 0;
}
