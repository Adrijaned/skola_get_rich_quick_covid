#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <vector>

void error(char const *msg) {
  perror(msg);
  exit(1);
}

/**
 * Stolen from https://stackoverflow.com/a/123724/6546996
 * @param s
 */
void trim(char *s) {
  char *p = s;
  int l = strlen(p);
  if (l == 0)return;
  while (isspace(p[l - 1])) p[--l] = 0;
  while (*p && isspace(*p)) ++p, --l;

  memmove(s, p, l + 1);
}

/**
 * This is basically a state machine, because fuck sanity.
 * @param sockFd
 */
void runGame(int sockFd) {
#define w(text) write(sockFd, text, strlen(text));
#define f write(sockFd, "------------> ", 14);
#define g bzero(buff, 255); if (!read(sockFd, buff, 255)) {printf("DC"); fflush(stdout); return;} trim(buff);
#define ifText(text) if (strcmp(text, buff) == 0) goto
    char buff[256];
    w("Welcome to Get Rich Quick with COVID-19!\n")
  inHouse:
    w("You are standing in a house. You can either (see), (go )[object], or (go outside).\n")
    f
    g
    ifText("see") house_see;
    ifText("go outside") outHouse;
    ifText("go tap") tap;
    ifText("go cupboard") cupboard;
    ifText("go table") table;
    goto inHouse;
  house_see:
    w("You see a {tap}, a {cupboard}, and a {table}.\n")
    goto inHouse;
  outHouse:
    w("You are standing outside a house. You can either (see), (go )[object], or (go inside).\n")
    f
    g
    ifText("see") outHouse_see;
    ifText("go inside") inHouse;
    ifText("go sandpit") sandPit;
    ifText("go furnace") furnace;
    goto outHouse;
  outHouse_see:
    w("You see a {sandpit}, and a {furnace}.\n")
    goto outHouse;
  sandPit:
    w("You are standing by a sandpit. You can either (pick) a handful of sand, or (return).\n")
    f
    g
    ifText("pick") handful;
    ifText("return") outHouse;
    goto sandPit;
  handful:
    w("You are standing outside a house, with a hand full of sand. You can either (see), (go )[object], or (go inside).\n")
    f
    g
    ifText("see") handful_see;
    ifText("go inside") handful_inHouse;
    ifText("go sandpit") handful_sandPit;
    ifText("go furnace") handful_furnace;
    goto handful;
  handful_see:
    w("You see a {sandpit}, and a {furnace}.\n")
    goto handful;
  handful_inHouse:
    w("Before entering the house, you throw away the sand you are holding. You have recently swept the floor!\n")
    goto inHouse;
  handful_sandPit:
    w("As you return to the sandpit, you throw the sand back in. It was starting to get uncomfortable holding it.\n")
    goto sandPit;
  handful_furnace:
    w("You are standing by an electric melting furnace, completely operational and running. You can either (insert) the sand you are holding into the furnace, or you can (return).\n")
    f
    g
    ifText("insert") melting;
    ifText("return") handful;
    goto handful_furnace;
  melting:
    w("As you insert the sand into the furnace, you can watch it quickly melt into glass and automatically get cast into a form. Soon, you are holding a brand new glass vial in your hands.\n")
  vial:
    w("You are standing outside a house, with a glass vial in your hand. You can either (see), (go )[object], or (go inside).\n")
    f
    g
    ifText("see") vial_see;
    ifText("go inside") vial_inHouse;
    ifText("go sandpit") vial_sandPit;
    ifText("go furnace") vial_furnace;
    goto vial;
  vial_see:
    w("You see a {sandpit}, and a {furnace}.\n")
    goto vial;
  vial_sandPit:
    w("You are standing by a sandpit. With the vial in your hands, you can really only (return).\n")
    f
    g
    ifText("return") vial;
    goto vial_sandPit;
  furnace:
    w("You are standing by an electric melting furnace, completely operational and running. You can either (poke) it, or you can (return).\n")
    f
    g
    ifText("poke") poke;
    ifText("return") outHouse;
    goto furnace;
  poke:
    w("The furnace doesn't seem to mind your poking in the slightest. After a short while, you give up trying to irritate it.\n")
    goto furnace;
  vial_furnace:
    w("You are standing by an electric melting furnace, completely operational and running, with a glass vial in your hands. You can either (poke) it, or you can (return).\n")
    f
    g
    ifText("poke") vial_poke;
    ifText("return") vial;
    goto vial_furnace;
  vial_poke:
    w("The furnace doesn't seem to mind your poking in the slightest. After a short while, you give up trying to irritate it.\n")
    goto vial_furnace;
  tap:
    w("You are standing by a water tap. You can either (check) it, or (return).\n")
    f
    g
    ifText("check") check;
    ifText("return") inHouse;
    goto tap;
  check:
    w("The sink is connected to plumbing. The knob turns ok. The water feels somewhat wet, but that's kinda expected. Overall, a decent water tap. 8 stars out of 10.\n")
    goto tap;
  cupboard:
    w("You are standing next to a cupboard. You can either (look) inside, or (return).\n")
    f
    g
    ifText("look") look;
    ifText("return") inHouse;
    goto cupboard;
  look:
    w("There is just some food coloring inside, nothing extraordinary.\n")
    goto cupboard;
  table:
    w("You are standing next to a table. You can either (inspect) it, or (return).\n")
    f
    g
    ifText("inspect") inspect;
    ifText("return") inHouse;
    goto table;
  inspect:
    w("Four legs, a flat top, useful piece of furniture for storing finished items.\n")
    goto table;
  vial_inHouse:
    w("You are standing in a house, vial in your hands. You can either (see), (go )[object], or (go outside).\n")
    f
    g
    ifText("see") vial_house_see;
    ifText("go outside") vial;
    ifText("go tap") vial_tap;
    ifText("go cupboard") vial_cupboard;
    ifText("go table") vial_table;
    goto vial_inHouse;
  vial_house_see:
    w("You see a {tap}, a {cupboard}, and a {table}.\n")
    goto vial_inHouse;
  vial_table:
    w("You are standing next to a table, vial in your hands. You can either (inspect) the table, or (return).\n")
    f
    g
    ifText("inspect") vial_inspect;
    ifText("return") vial_inHouse;
    goto vial_table;
  vial_inspect:
    w("Four legs, a flat top, useful piece of furniture for storing finished items. You would not fetch much money just for the vial though. Better work on it a bit more first.\n")
    goto vial_table;
  vial_cupboard:
    w("You are standing next to a cupboard, vial in your hands. You can either (look) inside, or (return).\n")
    f
    g
    ifText("look") vial_look;
    ifText("return") vial_inHouse;
    goto vial_cupboard;
  vial_look:
    w("There is just some food coloring inside, nothing extraordinary. It says \"Mix into water.\" on the label.\n")
    goto vial_cupboard;
  vial_tap:
    w("You are standing by a water tap, vial in your hands. You can either (check) the tap, (fill) the vial with water, or (return).\n")
    f
    g
    ifText("check") vial_check;
    ifText("return") vial_inHouse;
    ifText("fill") water_fill;
    goto vial_tap;
  vial_check:
    w("The sink is connected to plumbing. The knob turns ok. The water feels somewhat wet, but that's kinda expected. Overall, a decent water tap. 8 stars out of 10.\n")
    goto vial_tap;
  water_fill:
    w("You have filled the vial with tap water.\n")
  water:
    w("You are standing in a house, vial of water in your hands. You can either (see), (go )[object], or (go outside).\n")
    f
    g
    ifText("see") water_see;
    ifText("go outside") water_out;
    ifText("go tap") water_tap;
    ifText("go cupboard") water_cupboard;
    ifText("go table") water_table;
    goto water;
  water_see:
    w("You see a {tap}, a {cupboard}, and a {table}.\n")
    goto water;
  water_out:
    w("As you are stepping outside the house, you trip and spill all the water. Luckily the vial itself survived.\n")
    goto vial;
  water_tap:
    w("You are standing by a water tap, vial of water in your hands. It's not really clear to me what you want to do here, but you can either (check) the tap, or (return).\n")
    f
    g
    ifText("check") water_check;
    ifText("return") water;
    goto water_tap;
  water_check:
    w("The sink is connected to plumbing. The knob turns ok. The water feels somewhat wet, but that's kinda expected. Overall, a decent water tap. 8 stars out of 10.\n")
    goto water_tap;
  water_table:
    w("You are standing next to a table, vial of water in your hands. You can either (inspect) the table, or (return).\n")
    f
    g
    ifText("inspect") water_inspect;
    ifText("return") water;
    goto water_table;
  water_inspect:
    w("Four legs, a flat top, useful piece of furniture for storing finished items. So you wanna sell packaged water? I mean, you could, but that's too much work for too little profit. Better come up with something better.\n")
    goto water_table;
  water_cupboard:
    w("You are standing next to a cupboard, vial of water in your hands. You can either (look) inside, or (return).\n")
    f
    g
    ifText("look") water_look;
    ifText("return") water;
    goto water_cupboard;
  water_look:
    w("There is just some food coloring inside, nothing extraordinary. It says \"Mix into water.\" on the label.\n")
  water_cupboard_2:
    w("You are standing next to a cupboard full of food coloring, vial of water in your hands. You can either (look) inside, (mix) some of the coloring in the water, or (return).\n")
    f
    g
    ifText("look") water_look;
    ifText("mix") mix_t;
    ifText("return") water;
    goto water_cupboard_2;
  mix_t:
    w("You carefully mix the food coloring into the vial until you get a nice color.\n")
  mix:
    w("You are standing in a house, vial of colored water in your hands. You can either (see), (go )[object], or (go outside).\n")
    f
    g
    ifText("see") mix_see;
    ifText("go outside") water_out;
    ifText("go tap") mix_tap;
    ifText("go cupboard") mix_cupboard;
    ifText("go table") mix_table;
    goto mix;
  mix_see:
    w("You see a {tap}, a {cupboard}, and a {table}.\n")
    goto mix;
  mix_tap:
    w("You are standing by a water tap, vial of colored water in your hands. It's not really clear to me what you want to do here, but you can either (check) the tap, or (return).\n")
    f
    g
    ifText("check") mix_check;
    ifText("return") mix;
    goto mix_tap;
  mix_check:
    w("The sink is connected to plumbing. The knob turns ok. The water feels somewhat wet, but that's kinda expected. Overall, a decent water tap. 8 stars out of 10.\n")
    goto mix_tap;
  mix_cupboard:
    w("You are standing next to a cupboard, vial of colored water in your hands. You can either (look) inside, or (return).\n")
    f
    g
    ifText("look") mix_look;
    ifText("return") mix;
    goto mix_cupboard;
  mix_look:
    w("There is just some food coloring inside, nothing extraordinary. You don't really need to add more color to your water.\n")
    goto mix_cupboard;
  mix_table:
    w("You are standing next to a table, vial of colored water in your hands. You can either (inspect) the table, or (return).\n")
    f
    g
    ifText("inspect") mix_inspect;
    ifText("return") mix;
    goto mix_table;
  mix_inspect:
    w("Four legs, a flat top, useful piece of furniture for storing finished items. You think you could fetch a fair price for what you have in hands.\n")
  mix_table_2:
    w("You are standing next to a table, vial of colored water in your hands. You can either (inspect) the table, (place) the vial down on the table, or (return).\n")
    f
    g
    ifText("inspect") mix_inspect;
    ifText("place") place;
    ifText("return") mix;
    goto mix_table_2;
  place:
    w("You feel a sense of accomplishment as you put down the vial. This bottle of \"Totally Certified CoronaVirusStopper Vaccine\" should be able to fetch you a few hundred bucks! Well, time to make some more.\n")
    goto inHouse;
}

/**
 * Two locations: house, outside_house
 * house: tap (water), cupboard, table
 * outside_house: furnace, sand_pit
 * @return
 */
int main() {
  constexpr int portno = 10512;
  const int servSockFd = socket(AF_INET, SOCK_STREAM, 0);
  if (servSockFd < 0) {
    error("ERROR opening server socket");
  }
  int newSockFd;
  sockaddr_in serv_addr{}, cli_addr{};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(servSockFd, reinterpret_cast<const sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0)
    error("ERROR binding");
  listen(servSockFd, 5);
  std::vector<std::thread> threads{};
  while (true) {
    fflush(stdout);
    socklen_t clilen = sizeof(cli_addr);
    if ((newSockFd = accept(servSockFd, (struct sockaddr *)(&cli_addr), &clilen)) < 0)
      error("ERROR accept");
    threads.emplace_back(runGame, newSockFd);
  }
}
