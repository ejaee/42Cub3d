#include "cub3d.h"

void	parse(t_map *map, char *cub_file_path)
{
	char	*line;
	int		fd;
	int		identifier;

	/**
	����
	1. ���ϸ� Ȯ���� Ȯ��
	2. open Ȯ��
	
	3. getNextLine �� ���� ���پ� �о����
		- ������� Ȯ�� (NO, SO ...)
		- ������Ұ� �ƴ϶�� (��ȿ��)������ Ȯ��

		- validate �����ٸ� data ����
		- ������� �� �������� ������Ҷ�� �̸� ���� ���
		- �̹� ���� �ִٸ� �ߺ������� ����
		- ������Ҹ� �ǳ� ���� ����
		- xpm ������ ���������Ƿ� open �Ͽ� ��ȿ�� �������� Ȯ��
		- ������ ũ�⸸ŭ �Ҵ� ft_strdup()

		- ������� �� �����̶�� ���� ��ȿ�� üũ
		- �̹� ���� �ִ��� Ȯ��
		- ������Ҹ� �ǳ� ���� ����
		- .�� �������� ���� ������
		- �������� Ȯ��
		- �迭�� 3ĭ �Ѿ���� Ȯ��
		- ���ڸ� ���ڷ� ����
		- ������ ���ڰ� 0 ~ 255 ���ڰ� �ƴ� ��� Ȯ��
		- ������� = r * 256 * 256 + g * 256 + b
		- ������ڸ� ����
		- F, C �� ���� ���ڸ� ����

	4. map �� ���Դµ� �� ������ �ϳ��� ����Ǿ����� ������ �ȵȴ�
		���� ��� ���� ����Ǿ����� Ȯ��, ������ ���� �ٸ��� Ȯ��
		map�� 1���� �迭�� �̾� ���̱� strjoin()
		���� ������ �о��ٸ� parse map

		���� �������� split
		row ���� col ������ ft_strlen() ���� ���ϱ�
		map �� row col ����


	5. �� �Ҵ��� �Ϸ�Ǿ��ٸ� map setting
		1 �̳� ������ ������ �׳� �Ѿ�� 0NSEW �� ������ setting
		0NSEW �� �׵θ��� ������ ����
		0NSEW �ֺ��� �����̸� ����
		0NSEW ���� 0�� �ƴ϶�� player position save	
		�������ְ� �ش� ��ġ�� 0���� ����

	
	
	
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

