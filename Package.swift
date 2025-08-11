// swift-tools-version: 6.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "knnio",
    products: [
        .library(
            name: "KNNIOCore",
            targets: [
                "KNNIOCore"
            ]),
        .library(
            name: "KNNIOPosix",
            targets: [
                "KNNIOPosix"
            ]),
    ],
    dependencies: [
//        .package(url: "https://github.com/kainian/knfoundation.git", branch: "main"),
        .package(path: "../knfoundation")
    ],
    targets: [
        .target(
            name: "KNNIOCore",
            dependencies: [
                .product(name: "KNFoundation", package: "knfoundation")
            ]),
        .testTarget(
            name: "KNNIOCoreTests",
            dependencies: ["KNNIOCore"]),
        .target(
            name: "KNNIOPosix"),
        
    ]
)
