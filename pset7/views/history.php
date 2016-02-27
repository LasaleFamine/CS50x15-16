<div class="container">
    <div class="row">
        <div class="panel panel-primary filterable">
            <div class="panel-heading">
                <h3 class="panel-title">History</h3>
            </div>
             <table class="table table-striped">
                <thead>
                    <tr class="filters">
                        <?php foreach($history[0] as $key => $val): ?>
                            <th> <?= $key ?> </th>
                        <?php endforeach ?>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach($history as $prop): ?>
                            <tr>
                                <?php foreach($prop as $value): ?>
                                    <td><?= $value ?></td>
                                <?php endforeach ?>
                            </tr>
                    <?php endforeach ?>
                    
                   
                </tbody>
            </table>
        </div>
    </div>
</div>