#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <chrono>
#include <atomic>
#include <random>
#include <vector>

std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> imageQueue;
std::atomic<bool> producersDone(false);
std::atomic<int> totalGenerated(0);
std::atomic<int> totalProcessed(0);

// Función simulada para "producir" una imagen
std::string generateImage(int id) {
    return "Image " + std::to_string(id);
}

// Función del hilo productor
void producer(int id, int numImages) {
    for (int i = 0; i < numImages; ++i) {
        std::string image = generateImage(id * 100 + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));

        {
            std::unique_lock<std::mutex> lock(mtx);
            imageQueue.push(image);
            std::cout << "[Producer " << id << "] Generated: " << image << "\n";
            totalGenerated++;
        }

        cv.notify_one();
    }
}

// Función del hilo consumidor
void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [] {
            return !imageQueue.empty() || producersDone;
        });

        if (!imageQueue.empty()) {
            std::string image = imageQueue.front();
            imageQueue.pop();
            lock.unlock();

            std::cout << "[Consumer " << id << "] Processing: " << image << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 300));
            totalProcessed++;
        } else if (producersDone) {
            break;
        }
    }
}

int main() {
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    const int IMAGES_PER_PRODUCER = 25;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back(consumer, i + 1);
    }

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back(producer, i + 1, IMAGES_PER_PRODUCER);
    }

    for (auto& t : producers) {
        t.join();
    }

    producersDone = true;
    cv.notify_all();

    for (auto& t : consumers) {
        t.join();
    }

    std::cout << "\n[Summary] Total Images Generated: " << totalGenerated.load() << "\n";
    std::cout << "[Summary] Total Images Processed: " << totalProcessed.load() << "\n";

    return 0;
}