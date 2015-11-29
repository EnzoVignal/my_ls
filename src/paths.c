/*
** paths.c for my_ls in /home/boitea_r
** 
** Made by Ronan Boiteau
** Login   <boitea_r@epitech.net>
** 
** Started on  Wed Nov 25 21:15:48 2015 Ronan Boiteau
** Last update Sun Nov 29 20:47:07 2015 Ronan Boiteau
*/

#include "my.h"
#include "my_macro.h"
#include "ls_system.h"
#include "ls_args.h"


static int		_isdir(char *dir)
{
  int			idx;

  idx = 0;
  while (dir[idx])
    {
      if (dir[idx] == '/')
	return (TRUE);
      idx += 1;
    }
  return (FALSE);
}

static void		_arg_isfile(char *dir, int only_errors, int *extra_eol)
{
  DIR			*dir_ptr;
  struct dirent		*entry;
  int			found;
  int			idx;
  char			*filename;
  int			full_path;

  filename = my_strdup(dir);
  full_path = _isdir(dir);
  if (full_path == TRUE)
    {
      idx = my_strlen(dir) - 1;
      while (dir[idx] && dir[idx] != '/')
	{
	  dir[idx] = '\0';
	  idx -= 1;
	}
      idx = my_strlen(dir);
      while (filename[idx] && filename[idx] != '/')
	idx -= 1;
      filename += idx + 1;
      if ((dir_ptr = opendir(dir)) == NULL)
      	{
      	  if (only_errors == TRUE)
      	    my_put_error("ssss", "ls: cannot access ", dir, filename,
			 ": No such file or directory\n");
      	  return ;
      	}
    }
  else
    dir = my_strdup(".");
  dir_ptr = opendir(dir);
  found = FALSE;
  while ((entry = readdir(dir_ptr)) != NULL)
    {
      if (match(filename, entry->d_name)) 
 	{
  	  if (only_errors == FALSE)
  	    {
  	      if (full_path == TRUE)
  		my_putstr(dir);
  	      my_putstr(entry->d_name);
  	      my_putchar('\n');
  	      *extra_eol = TRUE;
  	    }
  	  found = TRUE;
  	}
    }
  if (found == FALSE && only_errors == TRUE)
    {
      if (full_path == TRUE)
  	my_put_error("ssss", "ls: cannot access ", dir, filename,
		     ": No such file or directory\n");
      else
	my_put_error("sss", "ls: cannot access ", filename,
		     ": No such file or directory\n");
    }
  return ;
}

int			_count_valid_paths(t_args *args, int errors)
{
  int			idx;
  int			paths;

  paths = 0;
  idx = 1;
  while (idx < args->argc)
    {
      if (args->argv[idx][0] != '-')
	paths += 1;
      idx += 1;
    }
  return (paths - errors);
}

int			_check_args(t_args *args,
				    int only_errors,
				    int *extra_eol)
{
  DIR			*dir_ptr;
  char			*dir;
  int			errors;

  errors = 0;
  args->idx = 1;
  while (args->idx < args->argc)
    {
      if (args->argv[args->idx][0] != '-')
	{
	  dir = my_strdup(args->argv[args->idx]);
	  if ((dir_ptr = opendir(dir)) == NULL)
	    {
	      _arg_isfile(dir, only_errors, extra_eol);
	      errors += 1;
	    }
	  free(dir);
	  closedir(dir_ptr);
	}
      args->idx += 1;
    }
  return (errors);
}
