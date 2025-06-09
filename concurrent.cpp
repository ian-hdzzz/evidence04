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
#include <cassert>
#include <set>

// Global variables for synchronization
std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> imageQueue;
std::atomic<bool> producersDone(false);
std::atomic<int> totalGenerated(0);
std::atomic<int> totalProcessed(0);

// Variables for validations
std::mutex validation_mtx;
std::set<std::string> generatedImages;
std::set<std::string> processedImages;
std::atomic<int> maxQueueSize(0);
std::atomic<int> currentQueueSize(0);

// Simulated function to "produce" an image
std::string generateImage(int id) {
    return "Image " + std::to_string(id);
}

// Producer thread function with validations
void producer(int id, int numImages) {
    for (int i = 0; i < numImages; ++i) {
        std::string image = generateImage(id * 100 + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));

        {
            std::unique_lock<std::mutex> lock(mtx);
            imageQueue.push(image);
            currentQueueSize++;
            
            // Update maximum queue size
            int current = currentQueueSize.load();
            int max = maxQueueSize.load();
            while (current > max && !maxQueueSize.compare_exchange_weak(max, current)) {
                max = maxQueueSize.load();
            }
            
            std::cout << "[Producer " << id << "] Generated: " << image << "\n";
            totalGenerated++;
            
            // Registration for validation
            {
                std::lock_guard<std::mutex> val_lock(validation_mtx);
                generatedImages.insert(image);
            }
        }

        cv.notify_one();
    }
}

// Consumer thread function with validations
void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [] {
            return !imageQueue.empty() || producersDone;
        });

        if (!imageQueue.empty()) {
            std::string image = imageQueue.front();
            imageQueue.pop();
            currentQueueSize--;
            lock.unlock();

            std::cout << "[Consumer " << id << "] Processing: " << image << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 300));
            totalProcessed++;
            
            // Registration for validation
            {
                std::lock_guard<std::mutex> val_lock(validation_mtx);
                processedImages.insert(image);
            }
        } else if (producersDone) {
            break;
        }
    }
}

// Function to validate data integrity
bool validateDataIntegrity() {
    std::cout << "\n=== DATA INTEGRITY VALIDATION ===\n";
    
    bool allProcessed = (generatedImages == processedImages);
    std::cout << "✓ All generated images were processed: " 
              << (allProcessed ? "YES" : "NO") << "\n";
    
    std::cout << "✓ Images generated: " << generatedImages.size() << "\n";
    std::cout << "✓ Images processed: " << processedImages.size() << "\n";
    std::cout << "✓ Maximum queue size: " << maxQueueSize.load() << "\n";
    
    return allProcessed;
}

// Function to execute a test case
void runTestCase(const std::string& testName, int numProducers, int numConsumers, 
                 int imagesPerProducer, bool shouldValidate = true) {
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "TEST CASE: " << testName << "\n";
    std::cout << "Producers: " << numProducers << ", Consumers: " << numConsumers 
              << ", Images per producer: " << imagesPerProducer << "\n";
    std::cout << std::string(50, '=') << "\n";
    
    // Reset global variables
    while (!imageQueue.empty()) imageQueue.pop();
    producersDone = false;
    totalGenerated = 0;
    totalProcessed = 0;
    maxQueueSize = 0;
    currentQueueSize = 0;
    generatedImages.clear();
    processedImages.clear();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // Create consumers first
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(consumer, i + 1);
    }

    // Create producers
    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(producer, i + 1, imagesPerProducer);
    }

    // Wait for producers to finish
    for (auto& t : producers) {
        t.join();
    }

    producersDone = true;
    cv.notify_all();

    // Wait for consumers to finish
    for (auto& t : consumers) {
        t.join();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "\n[SUMMARY] Total Images Generated: " << totalGenerated.load() << "\n";
    std::cout << "[SUMMARY] Total Images Processed: " << totalProcessed.load() << "\n";
    std::cout << "[SUMMARY] Execution time: " << duration.count() << " ms\n";
    std::cout << "[SUMMARY] Queue empty at end: " << (imageQueue.empty() ? "YES" : "NO") << "\n";
    
    if (shouldValidate) {
        bool success = validateDataIntegrity();
        std::cout << "\n[RESULT] Test case: " << (success ? "SUCCESSFUL ✓" : "FAILED ✗") << "\n";
    }
}

int main() {
    std::cout << "=== VALIDATION SYSTEM FOR STREAMING SIMULATION ===\n";
    std::cout << "Running test cases based on concurrency...\n";
    
    // Case 1: Concurrent access with limited capacity (Single consumer)
    runTestCase("Concurrent Access - Limited Capacity", 
                10, 1, 5);
    
    // Case 2: Excess producers over consumers
    runTestCase("Excess Producers over Consumers", 
                10, 2, 5);
    
    // Case 3: Multiple consumers, few producers
    runTestCase("Multiple Consumers - Few Producers", 
                2, 10, 3);
    
    // Case 4: Balanced distribution
    runTestCase("Balanced Distribution", 
                3, 3, 4);
    
    // Case 5: Clean system termination (Stress test)
    runTestCase("Clean Termination - Stress Test", 
                5, 3, 20);
    
    // Case 6: Single producer, multiple consumers
    runTestCase("Single Producer - Multiple Consumers", 
                1, 5, 10);
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "VALIDATION COMPLETED - ALL CASES EXECUTED\n";
    std::cout << std::string(60, '=') << "\n";
    
    return 0;
}

