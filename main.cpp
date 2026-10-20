
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <climits>
#include <limits>

using namespace std;

struct Edge {
    int to;
    int distance;

    Edge(int t, int d) {
        to = t;
        distance = d;
    }
};

class CampusNavigator {
private:
    vector<string> buildings;
    vector<vector<Edge> > graph;
    map<string, int> buildingID;
    vector<string> history;

    string readName(string message) {
        string name;
        cout << message;
        getline(cin >> ws, name);
        return name;
    }

    bool exists(string name) {
        return buildingID.find(name) != buildingID.end();
    }

public:
    void addBuildings() {
        int n;

        cout << "Enter number of buildings: ";

        if (!(cin >> n)) {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (n <= 0) {
            cout << "Invalid number of buildings.\n";
            return;
        }

        buildings.clear();
        graph.clear();
        buildingID.clear();
        history.clear();

        for (int i = 0; i < n; i++) {
            string name = readName(
                "Enter building " + to_string(i + 1) + ": ");

            if (exists(name)) {
                cout << "Building already exists. Try again.\n";
                i--;
                continue;
            }

            int id = (int)buildings.size();
            buildingID[name] = id;
            buildings.push_back(name);
            graph.push_back(vector<Edge>());
        }

        cout << "Buildings added successfully!\n";
    }

    void addPaths() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        int n;
        cout << "Enter number of paths: ";

        if (!(cin >> n) || n < 0) {
            cout << "Invalid number of paths.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        for (int i = 0; i < n; i++) {
            string source = readName("Enter source building: ");
            string destination =
                readName("Enter destination building: ");

            int distance;
            cout << "Enter distance in metres: ";

            if (!(cin >> distance)) {
                cout << "Invalid distance. Path skipped.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!exists(source) || !exists(destination)) {
                cout << "Building not found. Path skipped.\n";
                continue;
            }

            if (distance < 0) {
                cout << "Distance cannot be negative.\n";
                continue;
            }

            int u = buildingID[source];
            int v = buildingID[destination];

            graph[u].push_back(Edge(v, distance));
            graph[v].push_back(Edge(u, distance));

            cout << "Path added successfully!\n";
        }
    }

    void BFS() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        string start = readName("Enter starting building: ");

        if (!exists(start)) {
            cout << "Building not found.\n";
            return;
        }

        vector<bool> visited(buildings.size(), false);
        queue<int> q;

        int s = buildingID[start];
        visited[s] = true;
        q.push(s);

        cout << "BFS Traversal: ";

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            cout << buildings[u] << " ";

            for (int i = 0; i < (int)graph[u].size(); i++) {
                int v = graph[u][i].to;

                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        cout << "\n";
    }

    void DFS() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        string start = readName("Enter starting building: ");

        if (!exists(start)) {
            cout << "Building not found.\n";
            return;
        }

        vector<bool> visited(buildings.size(), false);
        vector<int> st;

        st.push_back(buildingID[start]);

        cout << "DFS Traversal: ";

        while (!st.empty()) {
            int u = st.back();
            st.pop_back();

            if (visited[u]) {
                continue;
            }

            visited[u] = true;
            cout << buildings[u] << " ";

            for (int i = (int)graph[u].size() - 1;
                 i >= 0; i--) {
                int v = graph[u][i].to;

                if (!visited[v]) {
                    st.push_back(v);
                }
            }
        }

        cout << "\n";
    }

    void shortestRoute() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        string start = readName("Enter source building: ");
        string finish = readName("Enter destination building: ");

        if (!exists(start) || !exists(finish)) {
            cout << "Building not found.\n";
            return;
        }

        int n = (int)buildings.size();
        int s = buildingID[start];
        int target = buildingID[finish];

        vector<long long> dist(n, LLONG_MAX);
        vector<int> parent(n, -1);
        vector<bool> visited(n, false);

        dist[s] = 0;

        for (int i = 0; i < n; i++) {
            int u = -1;

            for (int j = 0; j < n; j++) {
                if (!visited[j] &&
                    (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }

            if (u == -1 || dist[u] == LLONG_MAX) {
                break;
            }

            visited[u] = true;

            for (int j = 0; j < (int)graph[u].size(); j++) {
                int v = graph[u][j].to;
                int w = graph[u][j].distance;

                if (!visited[v] &&
                    dist[u] <= LLONG_MAX - w &&
                    dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }
            }
        }

        if (dist[target] == LLONG_MAX) {
            cout << "No route exists between these buildings.\n";
            return;
        }

        vector<int> path;

        for (int cur = target; cur != -1; cur = parent[cur]) {
            path.push_back(cur);
        }

        reverse(path.begin(), path.end());

        cout << "Shortest Route: ";

        for (int i = 0; i < (int)path.size(); i++) {
            cout << buildings[path[i]];

            if (i < (int)path.size() - 1) {
                cout << " -> ";
            }
        }

        cout << "\nTotal Distance: " << dist[target]
             << " metres\n";

        history.push_back(start + " -> " + finish);
    }

    void sortRoutes() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        string name = readName("Enter building name: ");

        if (!exists(name)) {
            cout << "Building not found.\n";
            return;
        }

        int u = buildingID[name];
        vector<Edge> routes = graph[u];

        sort(routes.begin(), routes.end(),
             [](const Edge &a, const Edge &b) {
                 if (a.distance == b.distance) {
                     return a.to < b.to;
                 }
                 return a.distance < b.distance;
             });

        if (routes.empty()) {
            cout << "No connected routes.\n";
            return;
        }

        cout << "Routes from " << name << ":\n";

        for (int i = 0; i < (int)routes.size(); i++) {
            cout << buildings[routes[i].to] << " - "
                 << routes[i].distance << " metres\n";
        }
    }

    void searchBuilding() {
        if (buildings.empty()) {
            cout << "Please add buildings first.\n";
            return;
        }

        string name = readName("Enter building name to search: ");

        if (!exists(name)) {
            cout << "Building not found.\n";
            return;
        }

        int u = buildingID[name];

        cout << "Building found: " << buildings[u] << "\n";
        cout << "Building ID: " << u + 1 << "\n";
        cout << "Connected buildings:\n";

        if (graph[u].empty()) {
            cout << "No connected buildings.\n";
        }

        for (int i = 0; i < (int)graph[u].size(); i++) {
            int v = graph[u][i].to;

            cout << buildings[v] << " - "
                 << graph[u][i].distance << " metres\n";
        }
    }

    void displayHistory() {
        if (history.empty()) {
            cout << "No route history available.\n";
            return;
        }

        cout << "Route Search History:\n";

        for (int i = 0; i < (int)history.size(); i++) {
            cout << i + 1 << ". " << history[i] << "\n";
        }
    }
};

int main() {
    CampusNavigator campus;
    int choice = 0;

    do {
        cout << "\n===== COLLEGE CAMPUS NAVIGATOR =====\n";
        cout << "1. Add Buildings\n";
        cout << "2. Add Paths\n";
        cout << "3. BFS Traversal\n";
        cout << "4. DFS Traversal\n";
        cout << "5. Find Shortest Route\n";
        cout << "6. Sort Routes by Distance\n";
        cout << "7. Search Building\n";
        cout << "8. Display Route History\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Program stopped.\n";
            break;
        }

        switch (choice) {
            case 1:
                campus.addBuildings();
                break;
            case 2:
                campus.addPaths();
                break;
            case 3:
                campus.BFS();
                break;
            case 4:
                campus.DFS();
                break;
            case 5:
                campus.shortestRoute();
                break;
            case 6:
                campus.sortRoutes();
                break;
            case 7:
                campus.searchBuilding();
                break;
            case 8:
                campus.displayHistory();
                break;
            case 9:
                cout << "Thank you for using Campus Navigator!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 9);

    return 0;
}
