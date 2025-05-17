#include "menu.h"
#include "name.h"
#include "version.h"
#include "LAN/LAN.h"
#include "JEU/jeu.h"
#include "globals.h"

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
BITMAP* curseur;
SAMPLE* musique;
static const int cinematic_frame_count = 239;

// Format de chemin, %d donnera 0,1,…,239
static const char cinematic_path_format[] =
    "../Projet/Cinématique/895d8352-f2bd-4821-8cf1-83f861da11d5-%d.bmp";

// Vérifie qu’un pointeur n’est pas NULL, quitte sinon
static void checkPtrNull(void* ptr, const char* err)
{
    if (!ptr) {
        allegro_message("%s", err);
        exit(EXIT_FAILURE);
    }
}
static SAMPLE* cinematic_music = NULL;

static void cinematic(int delay_ms)
{
    char filename[256];

    // ** 2) Chargement de la musique **
    cinematic_music = load_sample(
        "../Projet/Musiques/cinematicAudio.wav"
    );
    if (!cinematic_music) {
        allegro_message("Erreur chargement musique cinématique !");
        // pas fatal, mais on sort si besoin
        return;
    }

    // ** 3) Lecture en boucle (loop = 1) à plein volume **
    play_sample(cinematic_music,
                255,   // volume max
                128,   // pan central
                1000,  // vitesse normale
                0      // 1 = en boucle
    );

    // ** 4) Affichage des frames **
    for (int i = 0; i <= cinematic_frame_count; i++) {
        snprintf(filename, sizeof(filename), cinematic_path_format, i);

        BITMAP* bmp24 = load_bitmap(filename, NULL);
        if (!bmp24) {
            allegro_message("Erreur chargement frame %d !", i);
            break;
        }

        // Conversion 24→32 bits
        BITMAP* bmp32 = create_bitmap_ex(32, bmp24->w, bmp24->h);
        blit(bmp24, bmp32, 0,0, 0,0, bmp24->w, bmp24->h);
        destroy_bitmap(bmp24);

        // Affichage plein-écran
        stretch_blit(bmp32, screen,
                     0, 0, bmp32->w, bmp32->h,
                     0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        destroy_bitmap(bmp32);

        rest(delay_ms);
    }

    // ** 5) Arrêt et libération de la musique **
    stop_sample(cinematic_music);
    destroy_sample(cinematic_music);
    cinematic_music = NULL;
}

void initialisation_allegro();


int main() {
    initialisation_allegro();

    cinematic(50);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    char username[MAX_USERNAME];
    printf("\n");
    version();
    printf("\n");
    name(username);

    musique = load_sample("../Projet/Musiques/Menus.wav");
    if (!musique) {
        allegro_message("Erreur de chargement de la musique !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    play_sample(musique, 255, 128, 1000, 1);

    int choix = 0;
    while (choix != 3) {
        menu(&choix);
        switch (choix) {
        case 0: {
            init_nb_players_graphique();
            printf("\nChoix des joueurs (%d)\n", NB_JOUEURS);

            // 1) Saisie des pseudos
            Perso liste[NB_JOUEURS];
            for (int i = 0; i < NB_JOUEURS; i++) {
                liste[i] = init_player_graphique(i);
            }

            // 2) Sélection des personnages avec affichage intégré
            for (int i = 0; i < NB_JOUEURS; i++) {
                liste[i] = menu_selection_personnages_avec_transition(i);
            }
            // 3) Lancement du jeu local
            local(liste);
            break;
        }

        case 1: {
            init_nb_players_graphique();
            Perso self = init_player_graphique(-1);
            serveur();
            attendre_serveur();
            menu_waiting();
            client(username, self);
            break;
        }

        case 2: {
            init_nb_players_graphique();
            Perso self = init_player_graphique(-1);
            menu_waiting();
            client(username, self);
            break;
        }

        default:
            break;
        }
    }

    return 0;
}
END_OF_MAIN()

void redimensionner_curseur(BITMAP* curseur, int target_width, int target_height) {
    BITMAP* curseur_temp = create_bitmap(target_width, target_height);
    stretch_blit(curseur, curseur_temp, 0, 0, curseur->w, curseur->h, 0, 0, target_width, target_height);
    destroy_bitmap(curseur);
    curseur = curseur_temp;
}

void initialisation_allegro() {
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());

    get_desktop_resolution(&SCREEN_WIDTH, &SCREEN_HEIGHT);
    printf("Resolution detectee : %d x %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0) {
        allegro_message("Erreur initialisation audio : %s", allegro_error);
        exit(EXIT_FAILURE);
    }
    if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0) {
        allegro_message("Erreur : mode graphique non disponible !");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    curseur = load_bitmap("../Projet/Graphismes/Interface/Curseur/curseur.bmp", NULL);
    if (!curseur) {
        allegro_message("Erreur chargement curseur !");
        exit(EXIT_FAILURE);
    }

    BITMAP* temp = create_bitmap(32, 32);
    if (!temp) {
        allegro_message("Erreur création bitmap redimensionné !");
        exit(EXIT_FAILURE);
    }

    stretch_blit(curseur, temp, 0, 0, curseur->w, curseur->h, 0, 0, 32, 32);
    destroy_bitmap(curseur);
    curseur = temp;

    show_mouse(NULL);
}