#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#define TABLE_SIZE 100

typedef struct PhoneNumber {
  char number[20];
  struct PhoneNumber *next;
} PhoneNumber;

typedef struct Contact {
  char name[50];
  PhoneNumber *phoneNumbers;
  struct Contact *next;
} Contact;

Contact *hashTable[TABLE_SIZE];

unsigned int hashFunction(char *name) {
  unsigned int hash = 0;
  while (*name) {
    hash += *name++;
  }
  return hash % TABLE_SIZE;
}

void initializeHashTable() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashTable[i] = NULL;
  }
}

void adicionarContato() {
  char name[50];
  char phone[20];
  printf("Nome: ");
  scanf("%s", name);
  printf("Telefone: ");
  scanf("%s", phone);

  unsigned int index = hashFunction(name);
  Contact *current = hashTable[index];

  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      PhoneNumber *newPhoneNumber = (PhoneNumber *)malloc(sizeof(PhoneNumber));
      if (newPhoneNumber == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
      }
      strcpy(newPhoneNumber->number, phone);
      newPhoneNumber->next = current->phoneNumbers;
      current->phoneNumbers = newPhoneNumber;

      printf("Número de telefone adicionado ao contato existente.\n");
      return;
    }
    current = current->next;
  }

  Contact *newContact = (Contact *)malloc(sizeof(Contact));
  if (newContact == NULL) {
    printf("Erro de alocação de memória.\n");
    return;
  }
  strcpy(newContact->name, name);

  PhoneNumber *newPhoneNumber = (PhoneNumber *)malloc(sizeof(PhoneNumber));
  if (newPhoneNumber == NULL) {
    printf("Erro de alocação de memória.\n");
    free(newContact);
    return;
  }
  strcpy(newPhoneNumber->number, phone);
  newPhoneNumber->next = NULL;

  newContact->phoneNumbers = newPhoneNumber;

  newContact->next = hashTable[index];
  hashTable[index] = newContact;

  printf("Contato adicionado com sucesso.\n");
}

void buscarContato() {
  char name[50];
  printf("Nome: ");
  scanf("%s", name);

  unsigned int index = hashFunction(name);
  Contact *current = hashTable[index];

  clock_t start, end;
  double cpu_time_used;

  start = clock();

  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      end = clock();
      cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

      printf("Números de telefone de %s:\n", current->name);
      PhoneNumber *phoneCurrent = current->phoneNumbers;
      while (phoneCurrent != NULL) {
        printf("- %s\n", phoneCurrent->number);
        phoneCurrent = phoneCurrent->next;
      }
      printf("(tempo de busca: %.2f ms)\n", cpu_time_used);
      return;
    }
    current = current->next;
  }
  end = clock();
  cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
  printf("Contato não encontrado. (tempo de busca: %.2f ms)\n", cpu_time_used);
}

void removerContato() {
  char name[50];
  printf("Nome: ");
  scanf("%s", name);

  unsigned int index = hashFunction(name);
  Contact *current = hashTable[index];
  Contact *prev = NULL;

  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      PhoneNumber *phoneCurrent = current->phoneNumbers;
      while (phoneCurrent != NULL) {
        PhoneNumber *temp = phoneCurrent;
        phoneCurrent = phoneCurrent->next;
        free(temp);
      }
      if (prev == NULL) {
        hashTable[index] = current->next;
      } else {
        prev->next = current->next;
      }
      free(current);
      printf("Contato removido com sucesso.\n");
      return;
    }
    prev = current;
    current = current->next;
  }
  printf("Contato não encontrado.\n");
}

void exibirContatos() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Contact *current = hashTable[i];
    while (current != NULL) {
      printf("Nome: %s\n", current->name);
      printf("Números de telefone:\n");
      PhoneNumber *phoneCurrent = current->phoneNumbers;
      while (phoneCurrent != NULL) {
        printf("- %s\n", phoneCurrent->number);
        phoneCurrent = phoneCurrent->next;
      }
      printf("\n");
      current = current->next;
    }
  }
}

void liberarMemoria() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Contact *current = hashTable[i];
    while (current != NULL) {
      PhoneNumber *phoneCurrent = current->phoneNumbers;
      while (phoneCurrent != NULL) {
        PhoneNumber *tempPhone = phoneCurrent;
        phoneCurrent = phoneCurrent->next;
        free(tempPhone);
      }
      Contact *tempContact = current;
      current = current->next;
      free(tempContact);
    }
  }
}

int main() {
  int opcao;

  initializeHashTable();

  do {
    printf("\nEscolha uma opcao:\n");
    printf("1 - Adicionar contato\n");
    printf("2 - Buscar contato por nome\n");
    printf("3 - Remover contato\n");
    printf("4 - Exibir todos os contatos\n");
    printf("0 - Sair\n");
    printf("Digite uma opcao: ");

    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      adicionarContato();
      break;
    case 2:
      buscarContato();
      break;
    case 3:
      removerContato();
      break;
    case 4:
      exibirContatos();
      break;
    case 0:
      printf("Saindo...\n");
      break;
    default:
      printf("Opcao invalida! Tente novamente.\n");
    }
  } while (opcao != 0);

  liberarMemoria();
  return 0;
}
