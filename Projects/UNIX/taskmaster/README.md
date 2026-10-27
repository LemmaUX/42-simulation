# Taskmaster

Job control daemon — 42 UNIX project.

Leather vest, bullwhip, sadistic tendencies... LGTM.

## Autores

- Maxime Montinet — zaz@42.fr
- Florian Martinez — martine@42.fr

## Descripción

`taskmaster` es un demonio de *job control* al estilo de `supervisor`. Lee un
fichero de configuración, lanza los programas definidos como procesos hijos,
los mantiene vivos (reiniciándolos si es necesario), monitoriza su estado en
todo momento y ofrece una *control shell* interactiva en primer plano.

## Compilación

```sh
make
```

Requiere `gcc` / libc estándar (C11). Solo stdlib del lenguaje, sin librerías
externas.

## Uso

```sh
./taskmaster conf/taskmaster.conf
```

- `SIGHUP` → recarga la configuración sin matar procesos no modificados.
- `Ctrl+C` o el comando `exit` dentro de la shell → detención ordenada.

## Control shell

Comandos disponibles (al estilo `supervisorctl`):

| Comando                       | Descripción                                |
|-------------------------------|--------------------------------------------|
| `status`                      | Estado de todos los programas supervisados |
| `start <prog>`                | Arranca un programa                        |
| `stop <prog>`                 | Detiene un programa (graceful + kill)      |
| `restart <prog>`              | Reinicia un programa                       |
| `reload`                      | Recarga el fichero de configuración        |
| `tail <prog>`                 | Muestra las últimas líneas del log         |
| `help` / `?`                  | Ayuda                                      |
| `exit` / `quit`               | Detiene taskmaster y sale                  |

La shell incluye line editing básico (backspace, flechas ←/→, Home/End),
historial (↑/↓, persistido en `.taskmaster_history`) y autocompletado con TAB.

## Formato de configuración (YAML-like)

Un bloque por programa bajo `programs:`, clave: valor por línea. Ejemplo:

```yaml
programs:
  nginx:
    cmd: "/usr/local/bin/nginx -c /etc/nginx/test.conf"
    numprocs: 1
    umask: 022
    workingdir: /tmp
    autostart: true
    autorestart: unexpected   # true | false | unexpected
    exitcodes: [0, 2]
    startretries: 3
    starttime: 5
    stopsignal: TERM
    stoptime: 10
    stdout: /tmp/nginx.stdout # o "discard"
    stderr: /tmp/nginx.stderr # o "discard"
    env:
      STARTED_BY: taskmaster
      ANSWER: 42
```

Opciones por programa (obligatorias del subject):

- `cmd` — comando a ejecutar (se separa por espacios; argv[0] = binario)
- `numprocs` — nº de instancias a mantener corriendo
- `autostart` — arrancar al lanzar taskmaster (`true`/`false`)
- `autorestart` — `true` (siempre), `false` (nunca), `unexpected` (solo salidas inesperadas)
- `exitcodes` — códigos de salida considerados "esperados"
- `starttime` — segundos de vida para considerar el arranque exitoso
- `startretries` — nº de reintentos de arranque antes de abortar el programa
- `stopsignal` — señal para parada elegante (TERM, INT, QUIT, USR1, USR2, HUP)
- `stoptime` — segundos de espera antes de forzar `SIGKILL`
- `stdout` / `stderr` — fichero de redirección o `discard`
- `env` — variables de entorno adicionales
- `workingdir` — directorio de trabajo del hijo
- `umask` — máscara de permisos (octal)

## Logging

Todos los eventos relevantes (arranque, parada, reinicio, muerte inesperada,
recarga de configuración...) se escriben en `logs/taskmaster.log` con marca
de tiempo.

## Estructura del proyecto

```
taskmaster/
├── Makefile
├── authors
├── README.md
├── conf/taskmaster.conf   # ejemplo de configuración
├── includes/taskmaster.h
└── srcs/
    ├── main.c        # bucle principal, señales
    ├── config.c      # parser YAML-like
    ├── process.c     # spawn / waitpid / estados tipo supervisor
    ├── shell.c       # control shell (readline-like, historial, tab-completion)
    ├── log.c         # sistema de logging
    └── utils.c       # helpers (split, signales, time...)
```

## Bonus contemplados

- Autocompletado con TAB e historial persistente en la control shell.
- `tail <prog>` para inspeccionar logs por programa.
- Arquitectura cliente/servidor sobre UNIX socket (en progreso).
