// swift-tools-version: 6.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "knnio",
    products: [
        .library(
            name: "KNNIO",
            targets: [
                "KNNIO"
            ]),
    ],
    dependencies: [
//        .package(url: "https://github.com/kainian/knfoundation.git", branch: "main"),
        .package(path: "../knfoundation")
    ],
    targets: [
        .target(
            name: "KNNIO",
            dependencies: [
                .product(name: "KNFoundation", package: "knfoundation")
            ]),
        .testTarget(
            name: "KNNIOTests",
            dependencies: ["KNNIO"]),
    ]
)
