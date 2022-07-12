# Segmentation map

This software module generates a segmentation map that contains all detected and segmented elements of the circuit's input image. The structure of the output file follows a JSON schematic model compatible with an application for editing circuits, allowing it to be imported and used by that application.

This documentation uses the [TypeScript](https://www.typescriptlang.org/) syntax to describe the types of the schematic elements.

## Table of Contents

- [Segmentation map](#segmentation-map)
    - [Basic types](#basic-types)
        - [ID](#id)
        - [Properties](#properties)
        - [Position](#position)
    - [Secondary types](#secondary-types)
        - [Port](#port)
        - [Label](#label)
    - [Main Types](#main-types)
        - [Component](#component)
        - [Connection](#connection)
        - [Node](#node)
    - [Schematic](#schematic)
    - [Example](#example)

## Basic types

These are some simple helper types created to remove the redundancy of the more complex types.

### ID

Each element needs to have it's own ID so that it can be easily identified in the schematic. The only requirement is that the ID needs to be a **unique** string.

```ts
type ID = string;
```

The IDs generated are 128-bit numbers. For example, an ID can be something like this: `"8c0168ec-07fe-4b2c-96ee-4292ab34cfb2"`.

### Properties

Each element can have its own specific set of properties. What those properties are depends on the type of element.

```ts
type Properties = Object;
```

For example:

- A **DC Voltage Source** can have a property to describe its internal resistance.
- A **Resistor** can have its internal temperature has a property.
- ...

### Position

Each element must have a position so that it can be placed in the schematic. There are two types to specify the position:

- Global: The position relative to the origin of the schematic/canvas.
- Relative: The position relative to the origin of its parent element. Ranges from 0 to 1.

```ts
interface Position {
    x: number;
    y: number;
    angle?: number;
}

type GlobalPosition = Position
type RelativePosition = Position
```

> Note: `x` and `y` refer to the distance, in pixels, to the origin, and `angle` refers to the rotation, in degrees, of the element.

## Secondary types

These types make use of the [basic ones](#basic-types) (shown above), but are, nonetheless, not as complex, nor as important as the [main ones](#main-types). They simply describe small parts/modules of the aforementioned.

### Port

The `Port` is the connection point of a `Component`. It allows its owner to be connected to the circuit. It can only allow an `input`, an `output`, or be an `hybrid` (allow both).

```ts
interface Port {
    id: ID;
    type?: 'hybrid' | 'input' | 'output';
    position: RelativePosition;
    owner?: ID;
    connection?: ID;
}
```

It can also contain a reference to its `owner` and to the `connection` that is attached to it. It should also be noted that it can only hold one connection at a time - unlike a `Node`.

> Note: The `Port`s position has a unique twist to it. The values of `x` and `y` should range from 0 to 1, where 0 is the origin and 1 is the extreme of the component.

### Label

The `Label` is responsible for holding the text that visually differentiates each element.

```ts
interface Label {
    id: ID;
    name: string;
    value: string;
    unit: string;
    position: Position;
    isNameHidden?: boolean;
    isValueHidden?: boolean;
    owner?: ID;
}
```

It can hold a `name`, `value` and `unit`. For example, a **resistor** with the label "R1 = 10 kΩ" should have a label as follows:

```js
{
    ...
    "name": "R1",
    "value": "10k", // Or "10 k"
    "unit": "Ω"
    ...
}
```

The label can have some variations:

- No `value`: **"R1 (Ω)"**
- No `unit`: **"R1 = 10 k"**
- No `value` and no `unit`: **"R1"**

It can also contain a reference to its `owner`, and flags that controls if some parts of the label should be hidden, `isNameHidden` and `isValueHidden` respectively.

## Main types

These are the most complex types and hold the most data. They are the heart of this schema.

### Component

The `Component` can represent any kind of electrical component: a resistor, a capacitor, a voltage source, a voltimeter, etc.

```ts
interface Component {
    id: ID;
    type: string;
    fullName: string;
    position: GlobalPosition;
    label: Label;
    ports: Port[];
    properties?: Properties;
}
```

It is important to distinguish the `type` from the `fullName`:

- The `type`, as the name implies, describes the type of the component. For example, a resistor should have a `type` of **"R"**.
- The `fullName` describes a more human-friendly version of the `type`. For example, a resistor should have a `fullName` of **"Resistor"**

### Connection

The `Connection` represents the physical wire connection between `Component`s' `Port`s and `Node`s. It holds two main values: `start` and `end` which are the `ID`s of the two `Port`s or `Node`s that are connected.

```ts
interface Connection {
    id: ID;
    start: ID;
    end: ID;
    properties?: Properties;
}
```

### Node

`Node`s are just connection "hubs": points that can hold multiple connections - unlike `Port`s.

```ts
interface Node {
    id: ID;
    type?: 'real' | 'virtual';
    position: GlobalPosition;
    label: Label;
    connections?: ID[];
    properties?: Properties;
}
```

A `Node` is considered `"real"` if it holds **more than 2 connections** simultaneously. Otherwise it is just considered a `"virtual"` node.

## Schematic

Joining all of the `Main Types`, each into their specific array, it is then possible to describe the entire circuit schematic in just a single JSON file.

```ts
interface Schematic {
    components: Component[];
    connections: Connection[];
    nodes: Node[];
}
```

## Example

The following JSON file is an example of the segmentation map generated by this software module.

// TODO: Copy a JSON file generated and show the correspondent input image of the circuit.
