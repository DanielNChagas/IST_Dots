/* Fiz uma alteração na função RenderStats de modo a poder introduzir as mensagens de
 * vitoria, derroda e a mensagem inicial para clicar n para poder jogar, para nao ter
 * de criar outra função Render
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAX(a,b)    (((a)>(b))?(a):(b))
#define SQR(a)      (a)*(a)
#define M_PI 3.14159265
#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 650        // main game space size (era 850)
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 15      // maximum size of the board
#define MAX_COLORS 5
#define MARGIN 5
#define MAX_NAME_SIZE 100       // maximo numero de caracteres que pode conter o numero do utilizador
// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable( int, int, int [], TTF_Font *, SDL_Surface **, SDL_Renderer *);
void ProcessMouseEvent(int , int , int [], int , int *, int * );
void RenderPoints(int [][MAX_BOARD_POS], int, int, int [], int, SDL_Renderer *);
void RenderStats( SDL_Renderer *, TTF_Font *, int [], int , int);
void filledCircleRGBA(SDL_Renderer * , int , int , int , int , int , int );
// declaração da função que le os parametros de funcionamento do programa
void Parametros(char[], int*, int*, int*, int*, int[]);
// declaração da função que gera cores aleatoreamente no tabuleiro
void gera_cores (int [][MAX_BOARD_POS], int, int, int);
//declaração da função que identifica a primeira cor de uma jogada
void identifica_cores ( int[][MAX_BOARD_POS], int, int, int* ,int[], int[], int[], int*, int*);
// declaração da função que identifica todas as outras cores de uma jogada e guarda tanto a posição como a cor em 3 vetores distintos
void identifica_cores_2 (int[][MAX_BOARD_POS], int[], int[], int, int[], int, int*);
//declaração da função que identifica se a jogada é permitida
void validacao_jogada (int[], int, int*, int[],int[], int*);
//declaração da função que elimina as bolas caso a jogada for validada
void elemina_bolas (int[][MAX_BOARD_POS], int[], int[], int[], int, int, int*, int, int, int*, int[]);
//declaração da função que atualiza o tabuleiro
void organizacao_tabuleiro( int[] [MAX_BOARD_POS], int, int, int);
//declaração da função que identifica se na jogada foi feito um quadrado
void identifica_quadrados ( int[], int[], int*, int);
//declaração da função responsavel pela atualização da pontuação (grafica)
void atualiza_pontuacao (int[], int[], int, int*, int, int*, int*, int[]);
//declaração da função que se identifica se o jogador ganhou ou perdeu o jogo
void identifica_resultado (int[], int, int*, int*, int*, int*, int[], int);
//declaração da função que identifica se é necessário fazer shuffle
void shuffle (int[MAX_BOARD_POS][MAX_BOARD_POS], int, int,int, int*, int*);
//declaração da função responsavel por fazer shuffle
void estatisticas ( int[], int, int, int, char[], int, int);
//declaração da função responsável por renderizar a linhas que unem os pontos
void RenderLines(int [][MAX_BOARD_POS], int , int , int [], int ,int[], int [], int [], int ,int ,SDL_Renderer * );
//declaração da função responsável por copiar variaveis para possibilitar o undo
void copy_variables(int[][MAX_BOARD_POS], int [][MAX_BOARD_POS], int[] , int[] , int , int *, int, int );
//declaração da função undo
void undo(int [][MAX_BOARD_POS], int [][MAX_BOARD_POS], int [], int [], int *, int , int , int , int *, int *, int *, int *);
//declaração da função que renderiza mensagens
void RenderMenssages (SDL_Renderer *, TTF_Font *, int, int, int);
void new(int[][MAX_BOARD_POS], int, int , int , int*, int* , int*, int* , int* , int*,int [], int [], int[] , int );

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Daniel Chagas";
const char myNumber[] = "IST90043";
const int colors[3][MAX_COLORS] = {{255, 0, 0, 255, 255},{0, 255,0, 0, 255},{0, 0, 255, 255, 255}};

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( void )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;

    int delay = 200;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px = 0, board_size_px[2] = {0};
    int board_pos_x = 0, board_pos_y = 0;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int pt_x = 0, pt_y = 0;
    char name[MAX_NAME_SIZE] = {0};
//inicialização da variavel que so permite ao jogador jogar se estiver a 1
    int n = 0;
//inicialização da variavel correspondente ao numero de cores
    int ncolors = 0;
//inicialização da variavel correspondente ao numero de jogadas iniciais
    int nplays = 0;
//inicialização da variavel correspondente ao numero de jogadas ao longo do jogo
	int nplays_2 = 0;
//inicialização do vetor que guarda os objetivos iniciais relativamente a cada cor
    int objectives[MAX_COLORS] = {0};
//inicialização de outro vetor que guarda os objetivos com o decorrer do jogo
    int objectives_2[MAX_COLORS] = {0};
//inicialização de uma variável que serve para igualar os vetores acima inicializados
    int temp_obj = 0;
//inicialização de uma variavel que serve de contador
    int temp = 0;
//inicialização de uma variavel que serve de contador
    int temp_quad = 0;
//inicialização de um vetor que guarda as cores identificadas pelo rato
    int colores[STRING_SIZE] = {5};
// inicialização de um vetor que guarda as coordenadas x das cores identificadas pelo rato
    int x_pos [STRING_SIZE] = {0};
// inicialização de um vetor que guarda as coordenadas y das cores identificadas pelo rato
    int y_pos [STRING_SIZE] = {0};
//inicialização de uma variavel usada como flag para a identificação da validação da jogada
	int flag_val = 0;
//inicialização de uma variavel usada como flag para a identificação de quadrados
	int flag_quad = 0;
//inicialização de uma variavel usada como flag que dependendo do seu valor significa uma vitoria(1) ou uma derrota(-1)
	int flag_res = 0;
//inicialização de uma variavel usada como flag que indica o numero de vezes que ocorreu shuffle
	int flag_shuf = 0;
//inicialização de uma variavel usada como flag que indica se é para formar linhas enquantos os pontos estão a ser unidos
	int flag_line = 0;
//inicialização da variavel que guarda o numero de vitorias
	int vict = 0;
//inicialização da variavel que guarda o numero de derrotas
	int derr = 0;
//inicialização da variavel que guarda o numero de jogos jogados
	int njogos = 0;
//inicialização da variavel que guarda quando o jogo é uma vitoria ou uma derrota e no caso de vitorias o guarda o numero de jogadas restantes
	int stats[STRING_SIZE] = {-1};
//inicialização da variável que contem o numero de bolas eleminadas de cada cor caso seja feito um quadrado e exista bolas no seu interior
	int cor_eleminar[STRING_SIZE] = {0};
//inicialização de uma matrix usada para a função undo que guarda as posições da matriz na jogada anterior
	int board_u[MAX_BOARD_POS][MAX_BOARD_POS] ={{0}};
//inicialização de um vetor usado na função undo que guarda os objetivos da jogada anterior
	int objectives_u[STRING_SIZE] ={0};
//inicialização de uma variavel usada na função undo que guarda o numero de jogadas da função anterior
	int nplays_u = 0;
	
//função que le os parametros do jogo
    Parametros(name,&board_pos_x, &board_pos_y, &ncolors, &nplays, objectives );
    
	
	
    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);

    while( quit == 0)
    {
		srand(1234);
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {

                quit = 1;
                if(n==1) //se estiver um jogo a ocorrer
                {
					derr++; //incrementa o numero de derrotas
					stats[njogos] = -1;// é guardada no ficheiro uma derrota
				}
                estatisticas(stats, njogos, vict, derr, name,n, flag_res);//função que gera o ficheiro que guarda as estatisticas
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_n:{
						//função new
						
						new(board,  ncolors,  board_pos_y, board_pos_x, &n, &flag_res, &njogos, &derr, &nplays, &nplays_2,stats, objectives, objectives_2, temp_obj);
                    }break;
                    case SDLK_q:{
                        quit = 1;
                        if(n==1)//se estiver um jogo a decorrer
						{
							derr++;//invrementa o numero de derrotas
							stats[njogos] = -1;// guarda uma derrota no ficheiro
						}
                        estatisticas(stats, njogos, vict, derr, name,n, flag_res);//função que gera o ficheiro que guarda as estatisticas
					}
                    case SDLK_u:{
						if(nplays != nplays_2)
							undo(board, board_u, objectives_2,  objectives_u, &nplays_2, nplays_u,  board_pos_y, board_pos_x ,&derr, &vict, &n, &flag_res); break;
                    }
                    default:
                        break;
                }
            }
//condição que faz com que o jogador só possa jogar quando clicar na tecla n
            else if( n == 1)
            {
				 if ( event.type == SDL_MOUSEBUTTONDOWN )
				{
					ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
					printf("Button down: %d %d\n", pt_x, pt_y);
					//função que identifica a primeira cor da jogada
					identifica_cores(board, pt_x, pt_y, &temp, colores, x_pos, y_pos, &flag_line, &flag_shuf);
					
				}
				else if ( event.type == SDL_MOUSEBUTTONUP )
				{
					ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
					printf("Button up: %d %d\n", pt_x, pt_y);
					//função que valida a jogada
					validacao_jogada(colores, temp, &flag_val, x_pos, y_pos,&flag_line);
					if(flag_val == 1) // as variaveis só são copiadas se a jogada for válida
					{
						//função que copia variaveis para possibilitar a unção undo
						copy_variables(board, board_u, objectives_2 , objectives_u ,nplays_2 , &nplays_u, board_pos_y, board_pos_x );
					}
					//função que identifica quadrados na jogada
					identifica_quadrados(x_pos, y_pos, &flag_quad, temp);
					//função que elemina bolas
					elemina_bolas(board, x_pos, y_pos, colores,  flag_val, temp, &flag_quad, board_pos_y, board_pos_x, &temp_quad, cor_eleminar);
					//função que atualiza o tabuleiro
					atualiza_pontuacao(objectives_2, colores, temp, &temp_quad, flag_val, &flag_quad, &nplays_2, cor_eleminar); 
					//função que identifica o resultado do jogo (derrota ou  vitoria)
					identifica_resultado (objectives_2, nplays_2, &flag_res, &n, &vict, &derr, stats, njogos);
				}
				else if ( event.type == SDL_MOUSEMOTION )
				{
					ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
					printf("Moving Mouse: %d %d\n", pt_x, pt_y);
					//função que identifica as cores durante a jogada
					identifica_cores_2 (board, colores, x_pos, pt_x, y_pos, pt_y, &temp);
				}
			}
        }
        //função que organiza o tabuleiro
		organizacao_tabuleiro(board, board_pos_y, board_pos_x, ncolors);
		
		//função que faz shuffle
		shuffle(board, board_pos_y, board_pos_x,ncolors, &flag_shuf, &n);
		
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer);
       
        //função que faz render das linhas que unem os pontos
        RenderLines(board, board_pos_x, board_pos_y,board_size_px, square_size_px,colores, x_pos, y_pos,temp,flag_line, renderer );
       
        // render board
        RenderPoints(board, board_pos_x, board_pos_y, board_size_px, square_size_px, renderer);
        
        //função que renderiza os "STATS"
        RenderStats(renderer, serif, objectives_2, ncolors, nplays_2);
        
		//função que faz render das mensagens de vitoria derrota ou shuffle
        RenderMenssages(renderer, serif, n, flag_res, flag_shuf);
        
        // render in the screen all changes above
        SDL_RenderPresent(renderer);

        // add a delay
        SDL_Delay( delay );
    }
    
    

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

/**
 * ProcessMouseEvent: gets the square pos based on the click positions !
 * \param _mouse_pos_x position of the click on pixel coordinates
 * \param _mouse_pos_y position of the click on pixel coordinates
 * \param _board_size_px size of the board !
 * \param _square_size_px size of each square
 * \param _pt_x square nr
 * \param _pt_y square nr
 */
void ProcessMouseEvent(int _mouse_pos_x, int _mouse_pos_y, int _board_size_px[], int _square_size_px,
        int *_pt_x, int *_pt_y )
{
	int sq_x = 0, sq_y = 0;
    // corner of the board
    int x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    int y_corner = (TABLE_SIZE - _board_size_px[1] - 15);
    int circleX = 0, circleY = 0, circleR=0;
    int dist = 0;

    // verify if valid cordinates
    if (_mouse_pos_x < x_corner || _mouse_pos_y < y_corner || _mouse_pos_x > (x_corner + _board_size_px[0])
        || _mouse_pos_y > (y_corner + _board_size_px[1]) )
    {
        *_pt_x = -1;
        *_pt_y = -1;
        
       
        return;
    }

    // computes the square where the mouse position is
    sq_x = (_mouse_pos_x - x_corner) / (_square_size_px + SQUARE_SEPARATOR);
    sq_y = (_mouse_pos_y - y_corner) / (_square_size_px + SQUARE_SEPARATOR);

	circleX = x_corner + (sq_x+1)*SQUARE_SEPARATOR + sq_x*(_square_size_px)+(_square_size_px>>1);
    circleY = y_corner + (sq_y+1)*SQUARE_SEPARATOR + sq_y*(_square_size_px)+(_square_size_px>>1);
    circleR = (int)(_square_size_px*0.4f);

	dist = (int)floor(sqrt( SQR(_mouse_pos_x - circleX) + SQR(_mouse_pos_y - circleY) ));
	
	if (dist < circleR )
	{
		*_pt_x = sq_x;
		*_pt_y = sq_y;	
	}
	else
	{
		*_pt_x = -1;
		*_pt_y = -1;
	}
	
}

/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPoints(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer )
 {
    int clr, x_corner, y_corner, circleX, circleY, circleR;

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // renders the squares where the dots will appear
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
                // define the size and copy the image to display
                circleX = x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px)+(_square_size_px>>1);
                circleY = y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px)+(_square_size_px>>1);
                circleR = (int)(_square_size_px*0.4f);
                // draw a circle
                clr = _board[i][j];
                if (clr != -1 ) //para que as bolas sejam apagadas se o valor dos quadrados seja -1 ou -2 (-2 para eleminar as bolas fora da matriz que por uma razao que nao consegui identificar estavam a ser geradas)
					filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
        }
    }
}
 
/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */
void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}

/*
 * Shows some information about the game:
 * - Goals of the game
 * - Number of moves
 * \param _renderer renderer to handle all rendering in a window
 * \param _font font to display the scores
 * \param _goals goals of the game
 * \param _ncolors number of colors
 * \param _moves number of moves remaining
 */
void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int _goals[], int _ncolors, int _moves)
{
//inicialização da cor preto a usar no RenderText
	SDL_Color black = {0,0,0};
     //branco
    int height = 25;
    int space = 0;//inicialização da variavel que contem o espaço para escrever os objetivos
	int inic = 0;//inicialização da variavel que corresponde ao inicio do espaço onde são escritos os objetivos
	int i = 0;
	int rect_x = 0; //posição x dos retangulos por tras dos objetivos
// inicialização de strings (mensagens)
    char njogadas[] ="Numero de jogadas:";
    char objectives[] = "Objectives:";
//inicialização de strings para as quais vao ser convertidas inteiros
    char moves[STRING_SIZE] ={0};
    char obj_1 [STRING_SIZE]={0};
    char obj_2 [STRING_SIZE]={0};
    char obj_3 [STRING_SIZE]={0};
    char obj_4 [STRING_SIZE]={0};
    char obj_5 [STRING_SIZE]={0};
//conversão de inteiros para strings para poderem ser utilizadas na função render text
    sprintf(moves, "%d", _moves);
    sprintf(obj_1, "%d", _goals[0]);
    sprintf(obj_2, "%d", _goals[1]);
    sprintf(obj_3, "%d", _goals[2]);
    sprintf(obj_4, "%d", _goals[3]);
    sprintf(obj_5, "%d", _goals[4]);
    
    space = TABLE_SIZE - (39*MARGIN + 17*MARGIN) ; // calculo do espaço entre a mensagem "objetivos"(defenida à frente) e o final da board
    inic= 39*MARGIN + 17*MARGIN; // inicio do espaço onde vão ser escritos os objetivos
    
    SDL_Rect rect_1 = {5*MARGIN, height -4, 33*MARGIN, height+ 8};
    SDL_Rect rect_2 = {6*MARGIN , height -1, 31*MARGIN , height + 1};
    SDL_Rect rect_3 = {39*MARGIN, height -4, 17*MARGIN, height+ 8};
    SDL_Rect rect_4 = {40*MARGIN , height -1, 15*MARGIN , height + 1};
    
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); //muda a cor do retangulo para preto
    SDL_RenderFillRect (_renderer, &rect_1);//gera o retangulo
    SDL_RenderFillRect (_renderer, &rect_3);//gera o retangulo
    SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);//muda a cor do retangulo para cinzento
    SDL_RenderFillRect (_renderer, &rect_2);//gera o retangulo
	SDL_RenderFillRect (_renderer, &rect_4);//gera o retangulo
    // Gera o texto "numero de jogadas:" por cima do tabuleiro
    RenderText(7*MARGIN, height, njogadas, _font, &black, _renderer);
	// Escreve o numero de jogadas do jogador
    RenderText(33*MARGIN, height, moves, _font, &black, _renderer);
    //Gera o texto "Objectives" por cima do tabuleiro
    RenderText(40*MARGIN, height, objectives, _font, &black, _renderer);
    for(i=1;i<=_ncolors;i++)
	{
		rect_x =inic + i*(space/(_ncolors+1))-7*MARGIN;
		SDL_Rect rect_4 = {rect_x,height -4, 11*MARGIN, height+ 8};
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); //muda a cor do retangulo para preto
		SDL_RenderFillRect (_renderer, &rect_4);
		SDL_Rect rect_5 = {rect_x+ MARGIN,height -1, 9*MARGIN, height + 1};
		SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);//muda a cor do retangulo para cinzento
		SDL_RenderFillRect (_renderer, &rect_5);
	}
	
	
	if(_ncolors >=1)
	{
		//Gera um circulo vermelho
		filledCircleRGBA(_renderer,inic + (space/(_ncolors+1)), height+3*MARGIN -3,10 ,255, 0, 0);
		//Escreve o numero de bolas vermelhas a juntar
		RenderText(inic + (space/(_ncolors+1))-5*MARGIN, height, obj_1, _font, &black, _renderer);
	}
	if(_ncolors >=2)
	{
		// Escreve o numero de bolas verdes a juntar
		RenderText(inic + 2*(space/(_ncolors+1))-5*MARGIN, height, obj_2, _font, &black, _renderer);
		//Gera um circulo verde
		filledCircleRGBA(_renderer,inic + 2*(space/(_ncolors+1)), height+3*MARGIN-3,10 ,0, 255, 0);
	}
    if (_ncolors >=3)
    {
		// Escreve o numero de bolas azuis a juntar
		RenderText(inic + 3*(space/(_ncolors+1))-5*MARGIN, height, obj_3, _font, &black, _renderer);
		//Gera um circulo azuis
		filledCircleRGBA(_renderer,inic + 3*(space/(_ncolors+1)), height+3*MARGIN-3,10 ,0, 0, 255);
	}
	if(_ncolors >= 4)
	{
		// Escreve o numero de bolas magenta a juntar
		RenderText(inic + 4*(space/(_ncolors+1))-5*MARGIN, height, obj_4, _font, &black, _renderer);
		//Gera um circulo magenta
		filledCircleRGBA(_renderer,inic + 4*(space/(_ncolors+1)), height+3*MARGIN - 3,10 ,255, 0, 255);
	}
	if(_ncolors == 5)
	{
		// Escreve o numero de bolas brancas a juntar
		RenderText(inic + 5*(space/(_ncolors+1))-5*MARGIN, height, obj_5, _font, &black, _renderer);
		//Gera um circulo branco
		filledCircleRGBA(_renderer,inic + 5*(space/(_ncolors+1)), height+3*MARGIN - 3,10 ,255, 255, 255);
	}


   
}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // compute and adjust the size of the table and squares
    max_pos = MAX(_board_pos_x, _board_pos_y);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = _board_pos_x*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = _board_pos_y*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, 0,0,0,0);
    board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
    board.y = (TABLE_SIZE - _board_size_px[1] - 15);
    board.w = _board_size_px[0];
    board.h = _board_size_px[1];
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, 192,192,192,255 );

    // iterate over all squares
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px ;
            board_square.w = square_size_px + 2;
            board_square.h = square_size_px + 2;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "ISTDots", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}
void Parametros(char _name[] , int *_ncols, int *_nrows, int *_ncolors, int *_nplays, int  _colors[MAX_COLORS])
{
    int i =10, l =0;
//string que guarda o que é introduzido pelo utilizador quando é pedido o numero de linhas
    char rows[STRING_SIZE]= {0};
//caracter escrito depois de um numero
    char leter = '0';
//string que guarda o que é introduzido pelo utilizador quando é pedido o numero de colunas
    char cols[STRING_SIZE]= {0};
//string que guarda o que é introduzido pelo utilizador quando é pedido o numero de cores
	char colors[STRING_SIZE] ={0};
//string que guarda o que é introduzido pelo utilizador quando são pedidos os objetivos
	char objectives[STRING_SIZE] ={0};
//string que guarda o que é introduzido pelo utilizador quando é pedido o numero de jogadas
	char plays[STRING_SIZE] ={0};
//operação que le o nome do utilizador
    printf("Introduza o seu nome : ");
    while(i > 9)
    {
        i = 0;
        do
        {
            _name[i] = getchar(); // le letra a letra
            i++;
        }while(_name[i-1] != '\n'); // só deixa de ler quando fazem 'enter'
        if (i-1 > 9 || _name[0] == '\n') // se o numero de caracteres da string for superior a 8 ou se o primeiro caracter introduzido for 'enter' dá erro e pede um valor novo
        {
            printf("ERRO : o nome introduzido excede o numero de caracteres permitidos \n Volte a introduzir um nome :");
			i=10; //para se manter dentro do loop o valor do i é inicializado a 10 quando dá erro
		}
    }
// operação que lê o numero de linhas do tabuleiro
    while(*_nrows <5 || *_nrows >15 || leter != '\n')
    {	
		printf("Introduza o numero de linhas do tabuleiro (entre 5-15) : ");
		fgets(rows, STRING_SIZE, stdin);// guarda a string  introduzida pelo utilizador
		sscanf(rows, "%d%c", _nrows, &leter); // le os numeros e caracteres da string introduzida e guarda num inteiro
		if(*_nrows <5 || *_nrows >15 || leter != '\n' ) // se p numero não pertence á gama e existir caracteres depois do numero dá erro
			printf("ERRO: o valor introduzido não percente à gama permitida \n");
    }
    
//operação que lê o numero de colunas do tabuleiro
    while(*_ncols <5 || *_ncols >15 || leter != '\n')
    {
		printf("Introduza o numero de colunas do tabuleiro (entre 5-15) : ");
		fgets(cols, STRING_SIZE, stdin);// guarda a string  introduzida pelo utilizador
		sscanf(cols, "%d%c", _ncols, &leter);// le os numeros e caracteres da string introduzida e guarda num inteiro
		if(*_ncols <5 || *_ncols >15 || leter != '\n' ) // se p numero não pertence á gama e existir caracteres depois do numero dá erro
			printf("ERRO: o valor introduzido não percente à gama permitida \n");
    }
//operação que lê o numero de cores
    while(*_ncolors <1 || *_ncolors >5 || leter != '\n')
    {
		printf("Introduza o numero de cores (entre 1-5) : ");
		fgets(colors, STRING_SIZE, stdin);// guarda a string  introduzida pelo utilizador
		sscanf(colors, "%d%c", _ncolors, &leter);// le os numeros e caracteres da string introduzida e guarda num inteiro
		if(*_ncolors <1 || *_ncolors >5 || leter != '\n') // se p numero não pertence á gama e existir caracteres depois do numero dá erro
			printf("ERRO: o valor introduzido não percente à gama permitida \n");
    }
//operação que le os objetivos para cada cor
    for(l=0; l < *_ncolors; l++)
    {
        while(_colors[l] < 1 || _colors[l] > 99 || leter != '\n')
        {
            printf("Introduza o objetivo para a %d ª cor ( entre 1 e 99) :", (l+1));
			fgets(objectives, STRING_SIZE, stdin);// guarda a string  introduzida pelo utilizador
			sscanf(objectives, "%d%c", &_colors[l], &leter);// le os numeros e caracteres da string introduzida e guarda num inteiro
			if(_colors[l] < 1 || _colors[l] > 99 || leter != '\n') // se p numero não pertence á gama e existir caracteres depois do numero dá erro
				printf("ERRO: o valor introduzido não percente à gama permitida \n");
        }
    }
    while(*_nplays < 1 ||*_nplays > 99 || leter != '\n')
    {
		printf("Introduza o numero de jogadas (entre 1 e 99):");
		fgets(plays, STRING_SIZE, stdin);
		sscanf(plays, "%d%c", _nplays, &leter);
		if(*_nplays < 1 ||*_nplays > 99 || leter != '\n') // se p numero não pertence á gama e existir caracteres depois do numero dá erro
			printf("ERRO: o valor introduzido não percente à gama permitida \n");
     }
    
}
void gera_cores(int _board[][MAX_BOARD_POS], int _ncolors, int _nrows, int _ncols)
{
    int i = 0, j = 0;
    for (i=0;i < _nrows; i++) //ciclo encadeado para escrever numa matrix
    {
        for (j = 0; j < _ncols; j++)
        {
            _board[j][i] = (rand() % _ncolors); //geração de cores aleatorias distribuidas por todas as posições da matriz
        }
    }
   
}
void identifica_cores (int _board[MAX_BOARD_POS][MAX_BOARD_POS], int _x, int _y, int *_temp, int _colors[STRING_SIZE], int _xpos[], int _ypos[],int *_flag_line, int *_flag_shuf)
{
	int i = 0;
// loop que permite inicializar as coordenadas nos 3 vetores com o valor 5 (uma vez que nao existe cor associada ao valor 5)
	for ( i=0; i != STRING_SIZE; i++)
	{
		_colors[i] = 5;
		_xpos [i] = 5;
		_ypos [i] = 5;
		*_temp = 0;
	}
// identificação da primeira cor da jogada e das suas coordenadas que são guardadas em vetores distintos
    if(_x != -1 || _y != -1)
    {
		_colors[0] = _board [_x][_y];//guarda a cor da bola
		_xpos [0] = _x;//guarda a coordenada x
		_ypos [0] = _y;//guarda a coordenada y
		*_flag_line = 1; //flag que permite a criação de linhas para unir os pontos escolhidos
		*_flag_shuf = 0; // flag que indica que existem jogadas a ser feitas
	}
   
    

}
void identifica_cores_2 (int _board[MAX_BOARD_POS][MAX_BOARD_POS],int _colors[STRING_SIZE],int _x[STRING_SIZE],int _xpos, int _y[STRING_SIZE],int _ypos, int *_temp)
{
//condição que permite identificar se o rato mudou ou não de circulo no tabuleiro
    if (_x[*_temp] == _xpos && _y[*_temp] == _ypos)
        printf("same position \n");
//caso em que a posição não é a mesma 
    if ((_x[*_temp] != _xpos ||  _y[*_temp] != _ypos)  && (_xpos != -1 || _ypos != -1))
    {
		*_temp = *_temp +1;
// quando o rato muda de posição são guardadas nos vetores correspondentes as cordenadas e a cor dessa nova posição
		_x[*_temp] = _xpos;
		_y[*_temp] = _ypos;
        _colors[*_temp] = _board[_xpos][_ypos];
        if( *_temp>1)
        {
// Bloco responsavel por verificar se durante a jogada o utilizador voltou um movimento atras
			if(_x[*_temp] == _x[*_temp -2] && _y[*_temp] == _y[*_temp -2])
			{
// as posições que tinham sido gravadas são colocadas a 0 ou 5 (no caso das cores) e o contador volta atras 2 valores para voltar a gravar o novo movimento
				*_temp = *_temp -2;
				_x[*_temp + 1] = 0;
				_y[*_temp + 1] = 0;
				_colors[*_temp + 1] = 5;
				_x[*_temp + 2] = 0;
				_y[*_temp + 2] = 0;
				_colors[*_temp + 2] = 5;
				
				
			}
		}
    }
}
void validacao_jogada (int  *_colors, int _temp, int *_flag,int _xpos[], int _ypos[], int *_flag_line)
{
	int i = 0;

	*_flag = 0;//inicialização da flag que valida a jogada a 0
	*_flag_line = 0; // flag que a 0 impede a formação de linhas enquanto os pontos são unidos
	
//caso em que a jogada não é permitida, o valor da flag está a 0
	for (i = 0; i <_temp; i++)
	{
//se uma das cores selecionadas for diferente é enviada a flag com o valor 0 que corresponde a uma jogada não permitida
		if (_colors[i] != _colors[i+1] )
		{
			*_flag = 0;
			printf(" jogada não permitida\n");
			i = _temp; // igualo i a  _temp para forçar a saida do loop
		}
//se o movimento for diagonal é invalidado
		else if(_xpos[i] !=_xpos[i+1] && _ypos[i] != _ypos[i+1])
		{
			*_flag = 0;
			printf("movimento diagonal\n");
			i = _temp;// igualo i a  _temp para forçar a saida do loop
		}
// se o movimento sair do tabuleiro e  entrar na mesma coluna ou linha a jogada é invalidada
		else if (_xpos[i] - _xpos[i+1] <-1 || _xpos[i] - _xpos[i+1] > 1 || _ypos[i] - _ypos[i+1] <-1 || _ypos[i] - _ypos[i+1] > 1)
		{
			*_flag = 0;
			printf("movimento diagonal\n");
			i = _temp;
		}
//caso em que a jogada é permitida, o valor da flag está a 1
		else if (_colors [i] == _colors[i+1] ) // a jogada só é permitida se todas as cores guardadas no vetor forem iguais
			*_flag =1;

	}
}
void elemina_bolas(int _board[MAX_BOARD_POS][MAX_BOARD_POS], int _x[], int _y[], int _colors[], int _flag, int _temp, int *_flag_2, int _nrows, int _ncols, int *_temp_quad, int _cor_elem[])
{
// inicialização de variaveis usadas como contadores
	int i = 0;
	int j = 0;
	int a = 0;
//inicialização de uma variavel que identifica a cor que é eleminada dentro do quadrado
	int cor = 0;

// a função so é ativa caso a jogada for válida( flag = 1)
	if (_flag == 1)
	{
//as posições guardadas nos vetores _x e _y são colocadas a -1 na matriz(tabuleiro)
		for(i = 0; i <=_temp; i++)
			_board[_x[i]][_y[i]] = -1;
	}
//caso o jogador tenha feito um quadrado
	if(_flag == 1 && *_flag_2 == 1)
	{
		for (i=1;i < _nrows; i++) //ciclo encadeado para escrever/ler numa matriz menos a primeira linha e a primeira coluna que nao é necessário neste caso
			{
				for (j = 1; j < _ncols; j++)
				{
					if(_board[i][j] != -1 && (_board[i-1][j] == -1 &&_board[i][j-1] == -1))// se existirem bolas dentro do quadrado estas são eleminadas
					{
						for (a= j; a < _ncols; a++)
						{
							if( _board[i][a] == -1)
							{
								cor = _board[i][j]; // cor da bola é guardada na variavel cor
								_cor_elem[cor] = _cor_elem[cor] +1; // é incrementado na posição do vetor relativa à cor eleminada o numero de bolas a eleminar
								_board[i][j] = -1;// a sua posição torna-se -1 (inexistencia de cor 
							}
						}
					}		
				}
			}
		//ciclo que elemina na matrix todas as cores iguais á cor do quadrado
		for (i=0;i < _nrows; i++) //ciclo encadeado para escrever numa matrix
		{
			for (j = 0; j < _ncols; j++)
			{
				if (_board[j][i]  == _colors[0])// se a cor do circulo for igual à cor inicial da jogada, é eleminada
				{				
					_board[j][i] = -1;
					*_temp_quad = *_temp_quad +1; // incrementa o numero de bolas eleminadas devido ao quadrado
				}
			}
		}
	}
} 
void organizacao_tabuleiro (int _board[MAX_BOARD_POS][MAX_BOARD_POS],int _nrows,int _ncols, int _ncolors)
{
	int i = 0, j = 0;
	int  b = 0; //inicialização da variavel que  guarda a cor posição atual do tabuleiro
	srand(time(NULL)); 
    for (i=0;i <_ncols; i++) //ciclo encadeado para escrever ou ler elementos duma matrix
    {
        for (j = (_nrows - 1); j > 0; j--)// a matriz é lida coluna a coluna de baixo para cima e linha a linha da esquerda para a dizeira ou seja é lida coluna a coluna e depois muda de linha
        {
            if (_board[i][j] == -1)//se a cor foi eleminada
            {
				b=_board[i][j];//guarda a cor da posição atual do tabuleiro
				_board[i][j] = _board[i][j-1];// cor da posição acima é copiada para a atual
				_board[i][j-1] = b; // a cor da posição atual é copiada para a cor de cima
				//ou seja ha uma troca de cores entre a posição atual e a posiçã acima até ser
				// a primeira linha a conter todas as posições vazias
			}
				
			
        }
    }

	for (i = 0; i< _ncols; i++)//le a primeira linha do tabuleiro
   {
		if (_board [i][0] == -1)//se houver uma posição vazia
					_board [i][0] = rand() % _ncolors; //é gerada uma cor nova nessa posição
	}
}
void identifica_quadrados(int _x[], int _y[], int*_flag, int _temp)
{
// se duas coordenadas forem iguais então temos um quadrado
//logo al longo dos vetores que guardam as posições se encontrar duas posições iguais, o flag que
//identifica a existencia de um quadrado é posto a 1
	int i = 0, j = 0;
	for (i=0; i <= _temp;i++)
	{
		for (j = i+1; j <= _temp; j++)
		{
			if (_x[i] == _x[j] && _y[i] == _y[j])
				*_flag = 1;
		}
	}
}

void atualiza_pontuacao(int _objectives[],int _colors[], int _temp, int*_temp_quad,int  _flag,int *_flag2, int *_nplays, int _cor_elem[])
{
	int cor = 0;	//inicialização da variavel que guarda a cor  dos pontos eleminados
	int score =0; // inicialização da variavel que guarda os pontos da jogada
	int i = 0;
	cor = _colors[0]; //escolha da cor ddos pontos eleminados
	if(*_flag2 !=1)//uma vez que o _temp guarda o numero de movimentos e a flag2 esta ativa se for feito um quadrado ele nao guarda a posição inicial logo é necessário somar 1
		score = _temp + 1;
	else //se o for feito um quadrado o score consiste na soma dos quadrados eleminados pela escolha do utiliazdor com o numero de quadrados elemidados posteriormente 
		score =_temp + *_temp_quad;

	if (_flag == 1)
	{
		switch (cor)//consoante a cor inicial da jogada é alterada a pontuação respetiva a essa cor
		{
			case 0:
				_objectives[0] =_objectives[0] - score ; break; //vermelho
			case 1:
				_objectives[1] =_objectives[1] - score ; break;//verde
			case 2:
				_objectives[2] =_objectives[2] - score ; break;//azul
			case 3:
				_objectives[3] =_objectives[3] - score ; break;//rosa
			case 4:
				_objectives[4] =_objectives[4] - score ; break;//branco
		}
		
//caso o quadrado tenha bolas no seu interior
		for(cor = 0; cor<5;cor++)
		{
			_objectives[cor] = _objectives[cor] - _cor_elem[cor]; //são subtraidos o numero de bolas de cada cor guradadas no vetor _cor_elem[] aos objetivos
			_cor_elem[cor] = 0; //reinicialização do retor que guarda o numero de cores a eleminar de cada cor caso exista bolas dentro do quadrado
		}
//caso a pontuação seja inferior a 0 é igualada a 0
		for(i=0; i< MAX_COLORS; i++)
		{
			if(_objectives[i] < 0)
				_objectives[i] = 0;
		}
//a cada jogada é decrementado o numero de jogadas
		*_nplays = *_nplays - 1;
//reinicialização da variavel que guarda o numero de bolas da mesma cor a eleminar quando um quadrado é formado
		*_temp_quad = 0;
//reinicialização da flag 2
		*_flag2 = 0;
	}
}
void identifica_resultado(int _colors[], int _nplays, int *_flag_res, int *_n, int *_vict, int *_derr, int _stats[], int _njogos)
{ 
// se todos os objetivos estiverem a 0
	if (_colors[0] == 0 && _colors[1] == 0 && _colors[2] == 0 && _colors[3] == 0 && _colors[4] == 0)
	{
		*_flag_res = 1;//flag que identifica o resultado do jogo é posto a 1(que corresponde a vitoria)
		*_n=0; // a variavel que só permite o jogador jogar se estiver a 1 é posta a 0
		*_vict = *_vict+1; // é incrementado o numero de vitorias
		_stats[_njogos -1] = _nplays; // guarda o numero de jogadas restantes para terminar o jogo
		
	}
// se o numero de jogadas estiver a 0 e pelo menos um dos objetivos é diferente de 0
	if( _nplays == 0 && (_colors[0] != 0 || _colors[1] != 0 || _colors[2] != 0 || _colors[3] != 0 || _colors[4] != 0))
	{
		*_flag_res = -1;//flag que identifica o resultado do jogo é posto a -1(que corresponde a derrota)
		*_n=0;// a variavel que só permite o jogador jogar se estiver a 1 é posta a 0
		*_derr = *_derr + 1;// é incrementado o numero de derrotas
		_stats[_njogos -1] = -1; //guarda no ficheiro D (derrota)
	}
}
void shuffle (int _board[MAX_BOARD_POS][MAX_BOARD_POS],int _nrows, int _ncols,int _ncolors, int *_flag, int *_n)
{
	int i = 0, j = 0;
//inicialização de um vetor que guarda o numero de cada cor no tabuleiro antes de fazer shuffle
	int cores[MAX_COLORS] = {0};
// inicialização de variaveis que correspondem a coordenadas
	int x= 0, y= 0; 

//inicialização de uma variavel que contem o numero de casas em que não é possivel iniciar uma jogada
	int npos = 0;
	
    srand(time(NULL));
    for (i=1;i < _nrows; i++) //ciclo encadeado para escrever numa matrix
    {
        for (j = 1; j < _ncols; j++)
        {
            if(_board[i][j] != _board[i-1][j] && _board[i][j] != _board[i][j-1]) // se a cor na posição acima e á esquerda for diferente incrementa um no numero de posição em que não é permitida a jogada
				npos++;

        }
    }
    for(i=1 ; i<_nrows-1 ; i++)// analisa a primeira linha
    { 
		if(_board[i][0] != _board[i-1][0] && _board[i][0] != _board[i+1][0])
			npos++;
	}
	for(i=1 ; i<_ncols -1; i++) // analisa a primeira coluna
	{
		if(_board[0][i] != _board[0][i-1] && _board[0][i] != _board[0][i+1])
			npos++;
	}
    if (npos == (_ncols * _nrows- 3)) // se o numerp de posições for igual ao numero de posições da matrix - 3 (3 cantos que não são necessário fazer o teste) é feito shuffle
	{
		*_flag = 1;

	
//ciclo que em caso do shuffle seja necessário, guarda o numero de pontos de cada cor num vetor	
		for (i=0;i < _nrows; i++) //ciclo encadeado para escrever numa matrix
		{
			for (j = 0; j < _ncols; j++)
			{
				if(_board[i][j] != -1)
				{
					cores[_board[i][j]] = cores[_board[i][j]] + 1 ;
					_board[i][j] = -1; // depois de guardar o numero total de bolas de cada cor coloca o tabuleiro todo a -1 (inexistencia de cor)
			}
			}
		}
//ciclo que gera o mesmo numero de pontos da mesma cor que existia no tabuleiro anterior, numa nova distribuição
		for (i=0;i < _nrows; i++) //ciclo encadeado para escrever numa matrix
		{
			for(j=0; j < MAX_COLORS; j++)
			{
				for(i = 0 ; i < cores[j]; i++)
				{
					x= rand() % _nrows; // gera uma posição x aleatória
					y = rand() % _ncols; //gera uma posição y aleatória 
					if( _board[x][y] == -1) // se na posição (x,y) da matrix não existir uma cor, é colocada la uma ate o tabuleiro estar completo
						_board[x][y] = j;
					else
						i--;
				}
			}
		}
	}
	for(i=0; i < MAX_COLORS; i++) //reinicializa o vetor a 0
		cores[i] = 0;
	
}
void estatisticas( int _stats[], int _njogos, int _V, int _D, char _name[], int _n, int _flag)
{
	int a=0;
	FILE *fp; //inicialização de uma variavel que serve de contador
	fp = fopen ("stats.txt", "w"); // abertura do ficheiro
	if(fp == NULL) //teste de abertura do ficheiro
		printf("Erro na abertura do ficheiro\n");

	fprintf(fp, "Player's name : %s \n", _name); // escrita do nome do jogador no ficheiro
	if(_njogos != 0)
	{
		fprintf(fp, "Total games played : %d \n", _njogos);//escrita do numero de jogos jogados no ficheiro
		fprintf(fp, "Total Victorys : %d \n", _V);//escrita do numero de vitorias no ficheiro
		fprintf(fp, "Total Loses : %d \n", _D);//escrita do numero de derrotas no ficheiro
		for(a=0; a< _njogos - 1; a++)
		{
			fprintf(fp, "Game %d : ", a+1);// escrita do numero do jogo e se este foi uma vitoria ou uma derrota
			if(_stats[a ] == -1)//se foi derrota
				fprintf(fp, "D \n");
			else //se foi vitoria
				fprintf(fp, "%d V \n", _stats[a ]);
		}
//este bloco serve para corrigir um erro em que aparecia sempre que saia a meio do jogo uma Vitoria
		if(_n == 1 )
		{
			fprintf(fp, "Game %d : ", _njogos);
			fprintf(fp, "D \n");
		}
//caso o jogo seja vencido ou perdido
		else
		{
		fprintf(fp, "Game %d : ", _njogos);
		if(_flag ==1) // venceu
			fprintf(fp, "%d V \n", _stats[_njogos-1]);
		else if(_flag == -1) //perdeu
			fprintf(fp, "D \n");
		}
	}
// caso em que o jogador nao inicia um jogo
	else if(_njogos == 0)
		fprintf(fp, "No games played");
}
void RenderLines(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,int _board_size_px[], int _square_size_px,int _colores[], int _x[], int _y[],int _temp,int _flag_line, SDL_Renderer *_renderer )
{
    int x_corner, y_corner, rectX, rectY, i = 0;
	int rect_h = 0, rect_w=0,rect_x=0, rect_y=0;
    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1; 	
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);
    // define the size and copy the image to display
    if (_temp >0 )
		for(i=1; i	<=_temp; i++)
		{
//adapta o tamanho das linhas aos quadrados
    rectX = x_corner + (_x[i]+1)*SQUARE_SEPARATOR + _x[i]*(_square_size_px)+(_square_size_px>>1);
    rectY = y_corner + (_y[i]+1)*SQUARE_SEPARATOR + _y[i]*(_square_size_px)+(_square_size_px>>1);
   
			switch (_colores[0]) // consoante a primeira cor selecionada é escolhida a cor da linha
			{
				case 0:
					SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255); break;//vermelho
				case 1:
					SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255); break;//verde
				case 2:
					SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255); break;//azul
				case 3:
					SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255); break;//rosa
				case 4:
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); break;//branco
			}
	
			if((_x[i-1]!=_x[i] && _y[i-1] !=_y[i] )|| _x[i-1] -_x[i] <-1 ||_x[i-1] -_x[i] >1 ||_y[i-1] - _y[i] <-1 ||_y[i-1] - _y[i] >1) // se amabas as coordenadas x e y mudarem ao mesmo tempo ou se uma coordenada variar mais do que uma unidade, não renderiza linhas
				_flag_line = 0;
			else if(_x[i-1] == _x[i] + 1 && _y[i] == _y[i]) //se o movimento for para a direita
			{
				rect_h =_square_size_px /2;
				rect_w = _square_size_px + SQUARE_SEPARATOR;
				rect_x= rectX;
				rect_y= rectY - _square_size_px/4;
			}
			else if(_x[i-1] == _x[i] - 1 && _y[i-1] == _y[i] ) //se o movimento for para a esquerda
			{	
				rect_h =_square_size_px / 2;
				rect_w =(- _square_size_px - SQUARE_SEPARATOR);
				rect_x=rectX;
				rect_y=rectY - _square_size_px/4;
			}
			else if(_x[i-1] == _x[i]  && _y[i-1] == _y[i] + 1) //se o movimento for para baixo
			{
				rect_h =_square_size_px +SQUARE_SEPARATOR;
				rect_w = _square_size_px / 2;
				rect_x=rectX-_square_size_px/4;
				rect_y=rectY ;
			}
			else if(_x[i-1] == _x[i]  && _y[i-1] == _y[i] - 1) //se o movimento for para cima
			{
				rect_h =(-_square_size_px -SQUARE_SEPARATOR);
				rect_w = _square_size_px / 2;
				rect_x=rectX-_square_size_px/4;
				rect_y=rectY;
			}
			SDL_Rect rect = {rect_x,rect_y, rect_w, rect_h}; // adapta as dimensões e posião do quadrado consoante o movimento
			if( _flag_line !=0)//o retangulo só é gerado se a flag for diferente de 0, ou seja esta a decorrer uma seleção de jogada
				SDL_RenderFillRect(_renderer, &rect); // gera o retangulo
	}
	
}
 
//função que copia as posições da matriz para uma  nova matriz, os objetivos para um novo vetor e o numero de jogadas para uma nova variavel para poder executar a função undo
void copy_variables(int _board[MAX_BOARD_POS][MAX_BOARD_POS], int _board_2[MAX_BOARD_POS][MAX_BOARD_POS], int _objectives[], int _objectives_2[], int _nplays, int *_nplays_2, int _nrows, int _ncols)
{
	int i = 0, j = 0, a = 0;
	*_nplays_2 = _nplays;
    for (i=0;i < _nrows; i++) //ciclo encadeado para escrever/ler numa matrix
    {
        for (j = 0; j < _ncols; j++)
        {
            _board_2[j][i] = _board[j][i];//guarda as cores dos pontos no tabuleiro numa matrix secundária usada para a função undo
        }
    }
    for(a=0;a < MAX_COLORS;a++)
		_objectives_2[a] = _objectives[a];// guarda os objetivos num vetor secundário usado na função undo
}
void undo(int _board[MAX_BOARD_POS][MAX_BOARD_POS], int _board_2[MAX_BOARD_POS][MAX_BOARD_POS], int _objectives[], int _objectives_2[], int *_nplays, int _nplays_2, int _nrows, int _ncols, int *_derr, int *_vict, int *_n, int *_flag_res)
{
	int i = 0, j = 0, a = 0;
//copia o numero de jogadas da jogada anterior
	*_nplays = _nplays_2;
//copia as posições da jogada anterior para o tabuleiro atual
    for (i=0;i < _nrows; i++) //ciclo encadeado para escrever/ler numa matrix
    {
        for (j = 0; j < _ncols; j++)
        {
            _board[j][i] = _board_2[j][i];
        }
    }
//copia os objetivos da jogada anterior para a jogada atual
    for(a=0;a < MAX_COLORS;a++)
		_objectives[a] = _objectives_2[a];
	if (*_n == 0)
	{
//caso o jogador faça undo quando perdeu
		if (*_flag_res == -1)
		{
			*_derr = *_derr - 1;
			*_n = 1;	
		}
//caso o jogador faça undo quando ganhou
		if (*_flag_res == 1)
		{
			*_vict = *_vict - 1;
			*_n = 1;
		}
	}
}
void RenderMenssages (SDL_Renderer *_renderer, TTF_Font *_font, int _n, int _flag_res,int _flag_shuf)
{
//inicialização da cor preto a usar no RenderText
	SDL_Color black = {0,0,0};
//inicialização de uma altura
	int height= 25;
//inicialização das strings com as mensagens
	char iniciar[] = "PRESS N TO PLAY!"; // iniciar o jogo
    char derrota[] = "YOU LOST!";// derrota
    char vitoria[] = "YOU WON!";//vitoria
    char shuf[] = "YOU HAD NO MORE MOVES!";//shyffle
	
//inicialização dos rectanculos por tras das mensagens
	SDL_Rect rect_1 = {47*MARGIN, 3*height +2*MARGIN, 34*MARGIN , height +10};
    SDL_Rect rect_2 = {48*MARGIN, 3*height +2*MARGIN+4, 32*MARGIN , height +3};
    SDL_Rect rect_3 = {47*MARGIN, 3*height +2*MARGIN, 34*MARGIN , 2*height +10};
    SDL_Rect rect_4 = {48*MARGIN, 3*height +2*MARGIN+4, 32*MARGIN , 2*height +3};
    SDL_Rect rect_5 = {39*MARGIN, 3*height +3*MARGIN, 50*MARGIN , 2*height +3};
    SDL_Rect rect_6 = {40*MARGIN, 3*height +4*MARGIN, 48*MARGIN , height  +18};
//mensagem escrita quando é feito shuffle ("YOU HAD NO MOVES") e geração dos retangulos por tras da mensagem
	if(_flag_shuf == 1)
    {
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect (_renderer, &rect_5);
		SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);
		SDL_RenderFillRect (_renderer, &rect_6);
		RenderText(42*MARGIN, 4*height+3, shuf, _font, &black, _renderer);
	}
// Gera a mensagem ("press n to play") e gera os retangulos por tras da mensagem se n =0
	if (_n != 1)
	{
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect (_renderer, &rect_1);
		SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);
		SDL_RenderFillRect (_renderer, &rect_2);
		RenderText(50*MARGIN, 3*height + 3*MARGIN, iniciar, _font, &black, _renderer);
//gera a mensagem ("YOU LOST!") quando o jogador perde e gera os retangulos por tras da mensagem
		if (_flag_res == -1)
		{
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			SDL_RenderFillRect (_renderer, &rect_3);
			SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);
			SDL_RenderFillRect (_renderer, &rect_4);
			RenderText(56*MARGIN, 4*height + 3*MARGIN, derrota, _font, &black, _renderer);
			RenderText(50*MARGIN, 3*height + 3*MARGIN, iniciar, _font, &black, _renderer);
		}
//gera a mensagem ("YOU WIN")quando o jogador ganha e gera os retangulos por tras da mensagem
		else if (_flag_res == 1)
		{
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			SDL_RenderFillRect (_renderer, &rect_3);
			SDL_SetRenderDrawColor(_renderer, 192, 192, 192, 255);
			SDL_RenderFillRect (_renderer, &rect_4);
			RenderText(56*MARGIN, 4*height + 3*MARGIN, vitoria, _font, &black, _renderer);
			RenderText(50*MARGIN, 3*height + 3*MARGIN, iniciar, _font, &black, _renderer);
		}
	}	
}

void new(int _board[][MAX_BOARD_POS], int _ncolors, int _board_pos_y, int _board_pos_x, int*_n, int *_flag_res, int*_njogos, int *_derr, int *_nplays, int*_nplays2,int _stats[], int _objectives[], int _objectives2[], int _temp_obj)
{
	if((*_n==1 && *_flag_res == 0))//se estiver um jogo a ocorrer
	{
		*_derr= *_derr + 1;//incrementa o numero de derrotas
		_stats[*_njogos] = -1;// é guardada no ficheiro uma derrota
	}
	if(*_n == 0 && *_flag_res != 0)
		_stats[*_njogos] = -1; // é guardada no ficheiro uma derrota
    gera_cores(_board, _ncolors, _board_pos_y, _board_pos_x); //função que gera cores aleatorias
	*_nplays2 = *_nplays; // o numero de jogadas atuais é igualado ao inicial
    for(_temp_obj = 0;_temp_obj <MAX_COLORS; _temp_obj++) // ciclo que iguala os objetivos atuais aos iniciais
    {
		_objectives2[_temp_obj] = _objectives[_temp_obj];
	}
    *_n=1; //variavel que quando esta com o valor 1 permite o jogador jogar
    *_flag_res = 0;  // reinicialização da flag que indica o resultado do jogo
    *_njogos = *_njogos + 1; // sempre que é iniciado um novo jogo é incrementado o numero de jogos
}
/*							
						if((n==1 && flag_res == 0))//se estiver um jogo a ocorrer
						{
								derr++;//incrementa o numero de derrotas
								stats[njogos] = -1;// é guardada no ficheiro uma derrota
						}
						if(n == 0 && flag_res != 0)
							stats[njogos] = -1; // é guardada no ficheiro uma derrota
                        gera_cores(board, ncolors, board_pos_y, board_pos_x); //função que gera cores aleatorias
                        nplays_2 = nplays; // o numero de jogadas atuais é igualado ao inicial
                        for(temp_obj = 0;temp_obj <MAX_COLORS; temp_obj++) // ciclo que iguala os objetivos atuais aos iniciais
                        {
							objectives_2[temp_obj] = objectives[temp_obj];
						}
                        n=1; //variavel que quando esta com o valor 1 permite o jogador jogar
                        flag_res = 0;  // reinicialização da flag que indica o resultado do jogo
                       njogos++; // sempre que é iniciado um novo jogo é incrementado o numero de jogos
       */
