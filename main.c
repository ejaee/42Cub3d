#include "cub3d.h"

void	parse(t_map *map, char *cub_file_path)
{
	char	*line;
	int		fd;
	int		identifier;

	/**
	순서
	1. 파일명 확장자 확인
	2. open 확인
	
	3. getNextLine 을 통해 한줄씩 읽어오기
		- 구성요소 확인 (NO, SO ...)
		- 구성요소가 아니라면 (유효한)맵인지 확인

		- validate 끝났다면 data 저장
		- 구성요소 중 동서남북 구성요소라면 이를 먼저 고려
		- 이미 값이 있다면 중복됨으로 에러
		- 구성요소를 건너 값에 접근
		- xpm 파일이 적혀있으므로 open 하여 유효한 파일인지 확인
		- 파일의 크기만큼 할당 ft_strdup()

		- 구성요소 중 색깔이라면 색깔 유효성 체크
		- 이미 값이 있는지 확인
		- 구성요소를 건너 값에 접근
		- .를 기준으로 숫자 나누기
		- 숫자인지 확인
		- 배열이 3칸 넘어가는지 확인
		- 문자를 숫자로 변경
		- 각각의 숫자가 0 ~ 255 숫자가 아닐 경우 확인
		- 색깔숫자 = r * 256 * 256 + g * 256 + b
		- 색깔숫자를 리턴
		- F, C 에 색깔 숫자를 저장

	4. map 이 나왔는데 위 사항이 하나라도 저장되어있지 않으면 안된다
		따라서 모든 곳이 저장되었는지 확인, 색깔이 서로 다른지 확인
		map을 1차원 배열로 이어 붙이기 strjoin()
		파일 끝까지 읽었다면 parse map

		개행 기준으로 split
		row 별로 col 갯수를 ft_strlen() 으로 구하기
		map 의 row col 저장


	5. 맵 할당이 완료되었다면 map setting
		1 이나 공백을 만나면 그냥 넘어가고 0NSEW 를 만나면 setting
		0NSEW 가 테두리에 있으면 에러
		0NSEW 주변에 공백이면 에러
		0NSEW 에서 0이 아니라면 player position save	
		저장해주고 해당 위치는 0으로 변경

	
	
	
	*/

}

void	exit_with_error(char *message)
{
	write(1, message, ft_strlen(message));
	exit(FAIL);
}


int	parse()
{
	
}

int	main(int	argc, char**argv)
{
	t_game	game;

	if (argc != 2)
		exit_with_error("Error Message");

	parse();

	ft_memset(&map, 0, sizeof(t_map));
	parse(&map, argv[1]);
}

