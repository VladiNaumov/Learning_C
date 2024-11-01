/* 9.Process Credentials */

/*
 Каждый процесс связан с набором числовых идентификаторов пользователя (UID) и группы (GID).
 Иногда их называют идентификаторами процесса.

 В число этих идентификаторов входят:
1. real user ID and group ID - реальный ID пользователя и группы);
2. effective  UID and GID - действующий ID пользователя и группы;
3. saved set-user-ID and saved set-group-ID - сохраненный установленный ID пользователя (saved set-user-ID), сохраненный установленный ID группы (saved set-group-ID);
4. file-system UID and GID (Linux-specific) - характерный для Linux пользовательский.

 */